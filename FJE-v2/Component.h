#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include<stack>
#include "Factory.h"

class JsonComponent;
class JsonNode;
class JsonContainer;

// 观察者
class JsonVisitor {
public:
    void visit(JsonNode* node, Product* product, bool top, bool bottom, std::vector<bool> has_next);

    void visit(JsonContainer* container, Product* product, bool top, bool bottom, std::vector<bool> has_next);
};

// 迭代器
class CompositeIterator {
public:
    CompositeIterator(std::vector<std::shared_ptr<JsonComponent>> &children);
        

    bool hasNext() const {
        return current_ != temp_end;
    }
    
    JsonComponent* next();

    JsonComponent* current();
    
    JsonComponent* begin();

    bool hasBro() {
        auto temp = current_;
        return ++temp != temp_end;
    }

private:
    std::stack<std::vector<std::shared_ptr<JsonComponent>>::iterator> stack;
    std::vector< std::shared_ptr<JsonComponent>> data;
    std::vector<std::shared_ptr<JsonComponent>>::iterator begin_;
    std::vector<std::shared_ptr<JsonComponent>>::iterator current_;
    std::vector<std::shared_ptr<JsonComponent>>::iterator temp_end;
    std::vector<std::shared_ptr<JsonComponent>>::iterator end_;
};


// 抽象组件类
class JsonComponent {
public:
    virtual ~JsonComponent() {}
    virtual std::unique_ptr<CompositeIterator> createIterator() = 0;
    virtual void print(JsonVisitor* vistor, Product* product, bool bottom, bool top, std::vector<bool> has_next) = 0;
    virtual int getDepth()=0;
    virtual std::string getValue() = 0;
};


// 叶子节点类
class JsonNode : public JsonComponent {
private:
    
    std::string value;

    //bool is_leaf;
    int depth;

public:
    JsonNode(const std::string& val, int depth) : 
        value(val), depth(depth){
       

    }
    int getDepth() override { return depth; }
    void print(JsonVisitor* vistor, Product* product, bool top, bool bottom, std::vector<bool> has_next) override;

  
    std::string getValue() override { return value; }
    std::unique_ptr<CompositeIterator> createIterator() override {
        // 叶节点没有子节点抛出异常  
        throw std::runtime_error("Leaf cannot create iterator");
    }
};

// 容器节点类
class JsonContainer : public JsonComponent {
public:
    std::vector<std::shared_ptr<JsonComponent>> children;
    std::string value;
    int depth;
private:
    
public:
    JsonContainer(const std::string& val, int depth) :
        value(val), depth(depth) {
    }
    JsonContainer() { depth = -1; }
    std::string getValue() override { return value; }
    void addChild(const std::shared_ptr<JsonComponent>& child) {
        auto tmpChildren = &children;
        auto curDepth = this->depth;
        auto depth = child->getDepth();
        while (depth > curDepth+1) {
            tmpChildren = &(std::dynamic_pointer_cast<JsonContainer>((*tmpChildren)[tmpChildren->size() - 1])->children);
            curDepth += 1;
        }
       
        tmpChildren->push_back(child);
    }
    
    void print(JsonVisitor* vistor, Product* product, bool top = true,
        bool bottom = false, std::vector<bool> has_next = std::vector<bool>()) override;

    std::unique_ptr<CompositeIterator> createIterator() override {
        return std::make_unique<CompositeIterator>(children);
    }

    int getDepth() override { return depth; }
    
};


void JsonVisitor::visit(JsonNode* container, Product* product, bool top, bool bottom, std::vector<bool> has_next) {
    std::string result = product->style->getPre(top, bottom, has_next);
    auto value = product->icon->getIcon(false) + container->getValue();
    auto suff = product->style->getSuff(result, value);
    result += value + suff;
    std::cout << result << std::endl;
}
void JsonVisitor::visit(JsonContainer* container, Product* product, bool top, bool bottom, std::vector<bool> has_next) {
    std::string result = product->style->getPre(top, bottom, has_next);
    auto value = product->icon->getIcon(false) + container->getValue();
    auto suff = product->style->getSuff(result, value);
    result += value + suff;
    std::cout << result << std::endl;
}


JsonComponent*  CompositeIterator::begin() {
    return (*begin_).get();
}

CompositeIterator::CompositeIterator(std::vector<std::shared_ptr<JsonComponent>> &children)
: current_(children.begin()), end_(children.end()) {
    
    temp_end = end_;
    this->begin_ = children.begin();
    data = children;
}

JsonComponent* CompositeIterator::next() {
    if (hasNext()) {
        // 是容器则进入容器
        auto curContainer = std::dynamic_pointer_cast<JsonContainer>(*current_);
        if (curContainer) {
            stack.push(current_);
            current_ = curContainer->children.begin();
            temp_end = curContainer->children.end();

            return current();
        }

        current_++;
        // 用栈进行深度优先搜索
        if (current_ != temp_end) {
            return current();
        }
        else if (!stack.empty()) {
            
            while (!stack.empty() && !hasNext()) {
                current_ = stack.top();
                current_++;
                stack.pop();
                if (stack.empty()) {
                    temp_end = end_;
                }
                else {
                    temp_end = std::dynamic_pointer_cast<JsonContainer>(*stack.top())->children.end();
                }
            }
            return current();
        }
        else {
            current_ = std::vector<std::shared_ptr<JsonComponent>>::iterator();
            return nullptr;
        }
    }
    return nullptr;
}

JsonComponent* CompositeIterator::current(){
    if (!hasNext()) return nullptr;
   
    if (auto node = std::dynamic_pointer_cast<JsonNode>(*current_)) {
        return node.get();
    }
    else if (auto container = std::dynamic_pointer_cast<JsonContainer>(*current_)) {
        return container.get();
    }
    
    return nullptr;
}

void JsonNode::print(JsonVisitor* vistor,Product* product, bool top, bool bottom, std::vector<bool> has_next) {
    vistor->visit(this, product, top, bottom, has_next);
}

void JsonContainer::print(JsonVisitor* vistor, Product* product, bool top, bool bottom, std::vector<bool> has_next) {
    vistor->visit(this, product, top, bottom, has_next);
    
}


