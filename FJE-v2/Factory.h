#pragma once
#include <memory>
#include<map>
#include<unordered_map>
#include<functional>
#include"Product.h"

// 抽象工厂
class JsonFactory {
public:
    virtual~JsonFactory() {}
    virtual std::shared_ptr<Style> createStyle(const std::string& style) = 0;
    virtual std::shared_ptr<Icon> createIcon(std::string type) = 0;
};

// 注册工厂
class FactoryRegistrar {
public:
	static std::unordered_map<std::string, JsonFactory*> factories;

	static void Register(const std::string& type, JsonFactory* factoryFunc) {
		factories[type] = factoryFunc;
	}

    static JsonFactory* CreateFactory(const std::string& type) {
        auto it = factories.find(type);
        if (it != factories.end()) {
            return it->second;
        }
        return nullptr;
    }
};

std::unordered_map<std::string, JsonFactory*> FactoryRegistrar::factories;



class RectangleStyleFactory : public JsonFactory {
public:
    // 单例
    static RectangleStyleFactory* Instance() {

        return new RectangleStyleFactory();
    }

    RectangleStyleFactory() {
        
    }

    std::shared_ptr<Style> createStyle(const std::string& style) override {
        return this->CreateRectangleStyle();
    }
    std::shared_ptr<Icon> createIcon(std::string type) override {
        return std::make_shared<Icon>(type);
    }

private:
    std::shared_ptr<Style> CreateRectangleStyle() const {
        return std::make_shared<RectangleStyle>();
    }
};

class TreeStyleFactory : public JsonFactory {
public:
    // 单例
   static TreeStyleFactory* Instance() {
        return new TreeStyleFactory();
   }

    TreeStyleFactory() {
       
    }

    std::shared_ptr<Style> createStyle(const std::string& style) override {
        return this->CreateTreeStyle();
    }
    std::shared_ptr<Icon> createIcon(std::string type) override {
        return std::make_shared<Icon>(type);
    }

private:
    std::shared_ptr<Style> CreateTreeStyle() const {
        return std::make_shared<TreeStyle>();
    }
};


