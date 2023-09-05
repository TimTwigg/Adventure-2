// Updated 5 September 2023

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include "Storage.hpp"
#include "StringHelpers.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "Factory.hpp"
#include "Resource.hpp"
#include "CResource.hpp"
#include "json.hpp"
using json = nlohmann::json;

Storage::Storage(std::string name) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Storage: storage name required");
    json data = FileReader::getFromFile("storages.json", name);
    this->value = data["value"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<int>();
    this->type = OBJCLASS::STORAGE;
    recipe = data["recipe"].get<std::map<std::string, unsigned int>>();
    carry = data["carry"].get<bool>();
    capacity = data["capacity"].get<int>();
}

Storage::operator std::string() const noexcept {
    return "STORAGE, " + name + "; " + strHelp::join(stringifyContents(), ";");
}

bool Storage::isCarryable() const noexcept {
    return carry;
}

std::map<std::string, unsigned int> Storage::getRecipe() const noexcept {
    return recipe;
}

std::shared_ptr<Object> Storage::store(std::shared_ptr<Object> obj) {
    OBJCLASS objClass = obj->getType();
    if (objClass == OBJCLASS::RESOURCE || objClass == OBJCLASS::CRESOURCE) {
        Resource* i = static_cast<Resource*>(obj.get());
        for (auto it = contents.begin(); it != contents.end(); ++it) {
            Object* o = it->get();
            if (o->getName() == obj->getName()) {
                Resource* r = static_cast<Resource*>(o);
                if (getWeight() - weight + i->getWeight() <= capacity) {
                    r->add(i->getCount());
                    return nullptr;
                }
                else {
                    int indWeight = i->getWeight() / i->getCount();
                    int canTake = std::floor((capacity - getWeight() + weight) / indWeight);
                    i->remove(canTake);
                    r->add(canTake);
                    return obj;
                }
            }
        }
        // type is not yet in storage
        if (getWeight() - weight + obj->getWeight() <= capacity) {
            contents.push_back(obj);
            return nullptr;
        }
        else {
            int indWeight = i->getWeight() / i->getCount();
            int canTake = std::floor((capacity - getWeight() + weight) / indWeight);
            i->remove(canTake);
            if (objClass == OBJCLASS::RESOURCE) contents.push_back(std::shared_ptr<Object>(new Resource(i->getName(), canTake)));
            else contents.push_back(std::shared_ptr<Object>(new CResource(i->getName(), canTake)));
            return obj;
        }
    }
    else {
        // not a resource or cresource
        if (getWeight() - weight + obj->getWeight() <= capacity) {
            contents.push_back(obj);
            return nullptr;
        }
        else return obj;
    }
}

std::vector<std::string> Storage::stringifyContents() const noexcept {
    std::vector<std::string> sContents;
    for (std::shared_ptr<Object> o : contents) {
        sContents.push_back(o->operator std::string());
    }
    return sContents;
}

std::shared_ptr<Object> Storage::remove(std::string item, int count) {
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        Object* o = it->get();
        if (o->getName() == item) {
            if (o->getType() == OBJCLASS::RESOURCE || o->getType() == OBJCLASS::CRESOURCE) {
                Resource* r = static_cast<Resource*>(o);
                if (r->getCount() >= count) {
                    r->remove(count);
                    if (r->getCount() < 1) contents.erase(it);
                    if (r->getType() == OBJCLASS::RESOURCE) return std::shared_ptr<Object>(new Resource(r->getName(), count));
                    else return std::shared_ptr<Object>(new CResource(r->getName(), count));
                }
                else throw AdventureException("Storage::remove not enough to remove: " + item);
            }
            std::shared_ptr<Object> out = *it;
            contents.erase(it);
            return out;
        }
    }
    throw AdventureException("Storage::remove Could not find item: " + item);
}

int Storage::getWeight() const noexcept {
    int w = weight;
    for (auto o : contents) {
        w += o->getWeight();
    }
    return w;
}

int Storage::getCapacity() const noexcept {
    return capacity;
}

Storage* Storage::fromString(const std::string& code) {
    std::stringstream ss{code};
    Factory f;
    std::vector<std::string> v;
    std::string str;
    std::getline(ss, str, ',');
    v.push_back(str);

    while (ss.good()) {
        std::string str;
        std::getline(ss, str, ';');
        if (str.length() > 1) v.push_back(str);
    }

    Storage* s = new Storage(v[1]);
    for (int i = 2; i < v.size(); ++i) {
        strHelp::strip(v[i]);
        s->contents.push_back(std::shared_ptr<Object>(static_cast<Object*>(f.makeFromCode(v[i]))));
    }

    return s;
}