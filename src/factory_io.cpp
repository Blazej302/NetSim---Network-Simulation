//
// Created by blazej on 7/26/26.
//

#include "Factory.hpp"
#include "factory_io.hpp"

#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

static std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// helper to sort recipients according to specification
static std::vector<const IPackageReceiver*> get_sorted_receivers(const PackageSender& sender) {
    std::vector<const IPackageReceiver*> receivers;
    for (const auto& [receiver_ptr, prob] : sender.receiver_preferences_.get_preferences()) {
        receivers.push_back(receiver_ptr);
    }

    std::sort(receivers.begin(), receivers.end(), [](const IPackageReceiver* a, const IPackageReceiver* b) {
        if (a->get_receiver_type() != b->get_receiver_type()) {
            return a->get_receiver_type() < b->get_receiver_type();
        }
        return a->get_id() < b->get_id();
    });

    return receivers;
}

Factory load_factory_structure(std::istream& is)
{
    Factory factory;
    std::string line;

    while (std::getline(is, line)) {
        // Pomijamy puste linie oraz komentarze zaczynające się od ';'
        if (line.empty() || line[0] == ';') continue;

        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        std::string tag = tokens[0];
        std::map<std::string, std::string> params;

        for (std::size_t i = 1; i < tokens.size(); ++i) {
            auto pos = tokens[i].find('=');
            if (pos != std::string::npos) {
                params[tokens[i].substr(0, pos)] = tokens[i].substr(pos + 1);
            }
        }

        if (tag == "LOADING_RAMP") {
            ElementID id = std::stoi(params["id"]);
            TimeOffset di = std::stoi(params["delivery-interval"]);
            factory.add_ramp(Ramp(id, di));

        } else if (tag == "WORKER") {
            ElementID id = std::stoi(params["id"]);
            TimeOffset pt = std::stoi(params["processing-time"]);
            PackageQueueType qt = (params["queue-type"] == "LIFO") ? PackageQueueType::LIFO : PackageQueueType::FIFO;
            factory.add_worker(Worker(id, pt, std::make_unique<PackageQueue>(qt)));

        } else if (tag == "STOREHOUSE") {
            ElementID id = std::stoi(params["id"]);
            factory.add_storehouse(Storehouse(id));

        } else if (tag == "LINK") {
            std::string src = params["src"];   // np. "ramp-1"
            std::string dest = params["dest"]; // np. "worker-2"

            auto parse_node = [](const std::string& str) {
                auto dash = str.find('-');
                return std::make_pair(str.substr(0, dash), static_cast<ElementID>(std::stoi(str.substr(dash + 1))));
            };

            auto [src_type, src_id] = parse_node(src);
            auto [dest_type, dest_id] = parse_node(dest);

            IPackageReceiver* receiver = nullptr;
            if (dest_type == "worker") {
                receiver = &(*factory.find_worker_by_id(dest_id));
            } else if (dest_type == "store") {
                receiver = &(*factory.find_storehouse_by_id(dest_id));
            }

            if (src_type == "ramp") {
                factory.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(receiver);
            } else if (src_type == "worker") {
                factory.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(receiver);
            }
        }
    }

    return factory;
}

void save_factory_structure(const Factory& factory, std::ostream& os) {

    os << "; == LOADING RAMPS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << "\n";
    }

    os << "\n; == WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        std::string qtype = (it->get_queue_type() == PackageQueueType::LIFO) ? "LIFO" : "FIFO";
        os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_time() << " queue-type=" << qtype << "\n";
    }

    os << "\n; == STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }

    os << "\n; == LINKS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        auto receivers = get_sorted_receivers(*it);
        for (const auto* rec : receivers) {
            std::string type_str = (rec->get_receiver_type() == ReceiverType::WORKER) ? "worker" : "store";
            os << "LINK src=ramp-" << it->get_id() << " dest=" << type_str << "-" << rec->get_id() << "\n";
        }
    }
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        auto receivers = get_sorted_receivers(*it);
        for (const auto* rec : receivers) {
            std::string type_str = (rec->get_receiver_type() == ReceiverType::WORKER) ? "worker" : "store";
            os << "LINK src=worker-" << it->get_id() << " dest=" << type_str << "-" << rec->get_id() << "\n";
        }
    }
    os.flush();
}

void generate_structure_report(const Factory& factory, std::ostream& os) {
    os << "== LOADING RAMPS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING RAMP #" << it->get_id() << "\n";
        os << "  Delivery interval: " << it->get_delivery_interval() << "\n";
        os << "  Receivers:\n";

        auto receivers = get_sorted_receivers(*it);
        for (const auto* rec : receivers) {
            std::string rtype = (rec->get_receiver_type() == ReceiverType::WORKER) ? "worker" : "storehouse";
            os << "    " << rtype << " #" << rec->get_id() << "\n";
        }
        os << "\n";
    }

    os << "\n== WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        std::string qtype = (it->get_queue_type() == PackageQueueType::LIFO) ? "LIFO" : "FIFO";
        os << "WORKER #" << it->get_id() << "\n";
        os << "  Processing time: " << it->get_processing_time() << "\n";
        os << "  Queue type: " << qtype << "\n";
        os << "  Receivers:\n";

        auto receivers = get_sorted_receivers(*it);
        for (const auto* rec : receivers) {
            std::string rtype = (rec->get_receiver_type() == ReceiverType::WORKER) ? "worker" : "storehouse";
            os << "    " << rtype << " #" << rec->get_id() << "\n";
        }
        os << "\n";
    }

    os << "\n== STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE #" << it->get_id() << "\n\n";
    }
    os.flush();
}

void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time turn) {
    os << "=== [ Turn: " << turn << " ] ===\n\n";

    os << "== WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER #" << it->get_id() << "\n";

        const auto& pbuffer = it->get_processing_buffer();
        if (pbuffer.has_value()) {
            os << "  PBuffer: #" << pbuffer->get_id() << " (pt = " << (turn - it->get_package_processing_start_time() + 1) << ")\n";
        } else {
            os << "  PBuffer: (empty)\n";
        }

        os << "  Queue: ";
        const auto* q = it->get_queue();
        if (q->empty()) {
            os << "(empty)\n";
        } else {
            bool first = true;
            for (const auto& pkg : *q) {
                if (!first) os << ", ";
                os << "#" << pkg.get_id();
                first = false;
            }
            os << "\n";
        }

        const auto& sbuffer = it->get_sending_buffer();
        if (sbuffer.has_value()) {
            os << "  SBuffer: #" << sbuffer->get_id() << "\n";
        } else {
            os << "  SBuffer: (empty)\n";
        }
        os << "\n";
    }

    os << "\n== STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE #" << it->get_id() << "\n";
        os << "  Stock: ";

        const auto& stock = it->get_stock();
        if (stock.empty()) {
            os << "(empty)\n";
        } else {
            bool first = true;
            for (const auto& pkg : stock) {
                if (!first) os << ", ";
                os << "#" << pkg.get_id();
                first = false;
            }
            os << "\n";
        }
        os << "\n";
    }
    os.flush();
}