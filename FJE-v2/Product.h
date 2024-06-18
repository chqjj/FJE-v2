#pragma once
#ifndef PRODUCT_H
#define PRODUCT_H
#include<string>
#include<memory>
#include<vector>
#include<iostream>

// 抽象产品
class Style {
private:

public:
	
	virtual std::string getPre(bool top, bool bottom, std::vector<bool> has_next)=0;
	virtual std::string getSuff(std::string pre, std::string value)=0;
	
};

class Icon {
	
	std::string midNode;
	std::string leafNode;
public:
	Icon(std::string type) {
		if (type == "money") {
			midNode = "$";
			leafNode = "￥";
		}

	}
	std::string getIcon(bool is_leaf) {
		if (is_leaf) {
			return leafNode;
		}
		return midNode;
	}
};

class RectangleStyle : public Style {
	std::string putSpace(int count, std::string suffix) {
		return suffix.append(count, ' ');
	}
public:
	
	std::string getPre(bool top, bool bottom, std::vector<bool> has_next) {
		std::string prefix;
		int count = has_next.size()-1;

		while (count) {
			if (!bottom) {
				prefix += "│ ";
			}
			// 第一个和中间不一样
			else if (count == has_next.size() - 1) {
				prefix += "└ ";
			}
			else {
				prefix += "┴ ";
			}
			count -= 1;
		}
		if (top) {
			prefix += "┌ ";
		}
		else if (bottom) {
			if(has_next.size()==1) prefix += "└ ";
			else prefix += "┴ ";
		}
		else {
			prefix += "├ ";
		}
		return prefix;
	}
	std::string getSuff(std::string pre, std::string value) {
		std::string suffix;
		std::string end = "│ ";
		// 第一行和最后一行特殊处理
		if (pre.substr(0,2) == "┌") {
			end = "┐ ";
		}
		else if (pre.substr(0,2) == "└") {
			end = "┘ ";
		}
		int count = 40 - pre.size()/3*2 - value.size() - 2;
		suffix = putSpace(count % 2, suffix);
		count /= 2;
		
		while (count--) {
			suffix += "─ ";
		}
		
		auto result = suffix + end;
		
		return result;
	}
	
};

class TreeStyle : public Style {
private:
	
public:
	
	std::string getPre(bool top, bool bottom, std::vector<bool> has_next) {
		std::string prefix;
		int count = has_next.size();
		for (int i = 0; i < count-1; ++i) {
			prefix += has_next[i] ? "│ " : "  ";
		}
		if (has_next[count-1]) {
			prefix += "├ ";
		}
		else {
			prefix += "└ ";
		}
		return prefix;
	}
	std::string getSuff(std::string pre, std::string value) {
		// 树形没有后缀
		return "";
	}

};

// 产品组合
class Product {
public:
	std::shared_ptr<Style> style;
	std::shared_ptr<Icon> icon;
};


#endif