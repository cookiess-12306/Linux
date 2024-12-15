#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

enum Type
{
    PROTOTYPE_1 = 0,
    PROTOTYPE_2,
    PROTOTYPE_3
};

// 这相当于是原型的抽象类
class ProtoType
{
public:
    ProtoType() {}
    ProtoType(std::string prototype_name)
        : _prototype_name(prototype_name)
    {
    }

    virtual ~ProtoType() {}
    virtual ProtoType *Clone() const = 0;
    virtual void Method(float prototype_field)
    {
        _prototype_field = prototype_field;
        std::cout << "Call Method from " << _prototype_name << " with field : " << prototype_field << std::endl;
    }
protected:
    std::string _prototype_name; // 原型的名字
    float _prototype_field;  //原型参数
};

// 具体的原型类，也是原型的本体
class ConcreteProtoType1 : public ProtoType
{
public:
    ConcreteProtoType1(std::string prototype_name, float prototype_field)
        :ProtoType(prototype_name), _concrete_prototype_field1(prototype_field)
    {

    }
    ProtoType* Clone() const override
    {
        return new ConcreteProtoType1(*this);
    }
private:
    float _concrete_prototype_field1; //
};


class ConcreteProtoType2 : public ProtoType
{
public:
    ConcreteProtoType2(std::string prototype_name, float prototype_field)
        :ProtoType(prototype_name), _concrete_prototype_field2(prototype_field)
    {

    }
    ProtoType* Clone() const override
    {
        return new ConcreteProtoType2(*this);
    }
private:
    float _concrete_prototype_field2; //
};

class ConcreteProtoType3 : public ProtoType
{
public:
    ConcreteProtoType3(std::string prototype_name, float prototype_field)
        :ProtoType(prototype_name), _concrete_prototype_field3(prototype_field)
    {

    }
    ProtoType* Clone() const override
    {
        return new ConcreteProtoType3(*this);
    }
private:
    float _concrete_prototype_field3; //
};

// 原型工厂，在生产其他克隆体的时候更方便
// 其实在下面不是很能看出来，但如果他的原型构造参数再多一些，效果会明显很多
class ProtoTypeFactory
{
public:
    ProtoTypeFactory()
    {
        _prototypes[Type::PROTOTYPE_1] = new ConcreteProtoType1("PROTOTYPE_1", 50.f);
        _prototypes[Type::PROTOTYPE_2] = new ConcreteProtoType2("PROTOTYPE_2", 60.f);
        _prototypes[Type::PROTOTYPE_3] = new ConcreteProtoType2("PROTOTYPE_3", 70.f);

    }

    ~ProtoTypeFactory() 
    {
    delete _prototypes[Type::PROTOTYPE_1];
    delete _prototypes[Type::PROTOTYPE_2];
    delete _prototypes[Type::PROTOTYPE_3];
    }

    ProtoType* CreateProtoType(Type type)
    {
        return _prototypes[type]->Clone();
    }

private:
    std::unordered_map<Type, ProtoType *> _prototypes;
};


void Client(ProtoTypeFactory &factory)
{
    std::cout << "Let me create ProtoType 1\n";
    ProtoType* ProtoType1 = factory.CreateProtoType(Type::PROTOTYPE_1);
    ProtoType1->Method(10);
    delete ProtoType1;

    std::cout << "Let me create ProtoType 2\n";
    ProtoType* ProtoType2 = factory.CreateProtoType(Type::PROTOTYPE_2);
    ProtoType2->Method(20);
    delete ProtoType2;

    std::cout << "Let me create ProtoType 3\n";
    ProtoType* ProtoType3 = factory.CreateProtoType(Type::PROTOTYPE_3);
    ProtoType3->Method(30);
    delete ProtoType3;

}

int main()
{
    std::unique_ptr<ProtoTypeFactory> protofactory = std::make_unique<ProtoTypeFactory>();
    Client(*protofactory); 
    return 0;
}