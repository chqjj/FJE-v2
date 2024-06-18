#pragma once
#include"Component.h"
#include<fstream>
#include<direct.h>



class JsonBuilder {
	
	// 去除空白符
	std::string trim(std::string s) {
		if (!s.empty()) {
			s.erase(0, s.find_first_not_of(" "));
			s.erase(s.find_last_not_of(" ") + 1);
		}
		return s;
	}
	// 去除引号
	std::string deleteQuote(std::string s) {
		if (s[0] == '\"' && s[s.size() - 1] == '\"') {
			s.erase(0, 1);
			s.erase(s.size() - 1, 1);
		}
		return s;
	}

	std::shared_ptr<JsonComponent> process(std::string line, int& depth) {
		line = trim(line);

		// 根据:判断是否是节点
		auto pos = line.find(':');
		if (pos == std::string::npos) {
			if (line[0] == '}') depth -= 1;
			return nullptr;
		}

		std::string key = deleteQuote(trim(line.substr(0, pos)));
		std::string value = deleteQuote(trim(line.substr(pos + 1)));
		// 没有‘{’说明是叶子
		if (value != "{") {
			//std::cout << depth << std::endl;
			auto node = std::make_shared<JsonNode>(key + ':' + value, depth);    //new JsonNode(key + ':' + value, depth, true);
			return node;
		}
		else {
			//std::cout << depth << std::endl;
			auto node = std::make_shared<JsonContainer>(key, depth);
			depth += 1;
			return node;
		}
		return nullptr;

	}


public:
	JsonContainer buildJson(std::string route) {
		std::ifstream ifs;

		ifs.open(route, std::ios::in);
		JsonContainer json;
		int depth = 0;
		if (ifs.is_open())
		{

			std::string buf;
			
			while (getline(ifs, buf))
			{
				
				auto node = process(buf, depth);
				if (node) {
					json.addChild(node);
				}
			}
		}

		//关闭文件   
		ifs.close();
		return json;
	}
	Product* set(std::string styleName, std::string iconName){
		JsonFactory* factory = FactoryRegistrar::CreateFactory(styleName);
		auto style = factory->createStyle(styleName);
		auto icon = factory->createIcon(iconName);
		auto product = new Product();
		product->style = std::dynamic_pointer_cast<Style>(style);
		product->icon = std::dynamic_pointer_cast<Icon>(icon);
		return product;
	}

	void print(JsonContainer json, Product* product) {
		if (json.children.empty()) return;
		JsonVisitor* vistor = new JsonVisitor();

		auto iter = json.createIterator();
		auto cur = iter->begin();
		std::vector<bool> curDepth;
		int lastDepth = -1;
		// 迭代器遍历并输出
		while (iter->hasNext()) {
			auto currentComponent = iter->current();
			bool top = iter->begin() == currentComponent ? true : false;
			auto tmpNext = currentComponent;
			while (currentComponent->getDepth() <= lastDepth) { curDepth.pop_back(); lastDepth--; }
			lastDepth = currentComponent->getDepth();
			
			if (iter->hasBro()) {
				curDepth.push_back(true);
			}
			else {
				curDepth.push_back(false);
			}
	    

			bool bottom = iter->next() ? false : true;
			currentComponent->print(vistor, product, top, bottom, curDepth);
		}
	}
};

