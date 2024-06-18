
#include"json.h"
// 用于注册的辅助类  
class FactoryRegistrarInitializer {
public:
	FactoryRegistrarInitializer() {
		// 注册所有工厂  
		FactoryRegistrar::Register("rectangle", RectangleStyleFactory::Instance());
		FactoryRegistrar::Register("tree", TreeStyleFactory::Instance());
		     
	}
};

static FactoryRegistrarInitializer g_factoryRegistrarInitializer;



int main(int argc, char* argv[]) {
	std::string jsonFile = "json1.json";
	std::string styleName = "rectangle";
	std::string iconFamily = "";
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-f" && i + 1 < argc) {
			jsonFile = argv[++i];
		}
		else if (arg == "-s" && i + 1 < argc) {
			styleName = argv[++i];
		}
		else if (arg == "-i" && i + 1 < argc) {
			iconFamily = argv[++i];
		}
	}
	auto builder = JsonBuilder();
	auto jsonContainer = builder.buildJson(jsonFile);
	auto product = builder.set(styleName, iconFamily);
	
	builder.print(jsonContainer, product);

}