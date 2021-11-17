#include <iostream>

#include <catch.hpp>


// Data types

struct Circle
{
    double radius = 0;
};

struct Rectangle
{
    double width = 0;
    double height = 0;
};

// Painters

struct CirclePainter
{
    void paint(const Circle& circle)
    {
        std::cout << "Paint circle, radius = " << circle.radius << std::endl;
    }
};

struct CircleBoldPainter
{
    void paint(const Circle& circle)
    {
        std::cout << "PAINT CIRCLE, RADIUS = " << circle.radius << std::endl;
    }
};

struct RectanglePainter
{
    void paint(const Rectangle& rect)
    {
        std::cout << "Paint rectangle, width = "<< rect.width
            << ", height = " << rect.height << std::endl;
    }
};

// Serializers

struct CircleSerializer
{
    std::string serialize(const Circle&)
    {
        return "Serialized circle";
    }
};

struct CircleBinarySerializer
{
    std::string serialize(const Circle&)
    {
        return "Binary serialized circle";
    }
};

struct RectangleSerializer
{
    std::string serialize(const Rectangle&)
    {
        return "Serialized rectangle";
    }
};

// Type erasure

class Shape
{
    struct ShapeHolder
    {
        virtual ~ShapeHolder() = default;

        virtual void paint() = 0;
        virtual std::string serialize() = 0;
    };

    template<class ShapeType, class PainterType, class SerializerType>
    struct ShapeHolderImpl : ShapeHolder
    {
        ShapeHolderImpl(
            ShapeType&& shape,
            PainterType&& painter,
            SerializerType&& serilizer)
            : shape{std::forward<ShapeType>(shape)}
            , painter{std::forward<PainterType>(painter)}
            , serializer{std::forward<SerializerType>(serilizer)}
        {}

        void paint() override
        {
            painter.paint(shape);
        }

        std::string serialize() override
        {
            return serializer.serialize(shape);
        }

        ShapeType shape;
        PainterType painter;
        SerializerType serializer;
    };

public: // Type erasure public interface
    template<class ShapeType, class PainterType, class SerializerType>
    Shape(ShapeType&& shape, PainterType&& painter, SerializerType&& serializer)
    {
         holder = std::make_unique<ShapeHolderImpl<
                ShapeType, PainterType, SerializerType>>(
            std::forward<ShapeType>(shape),
            std::forward<PainterType>(painter),
            std::forward<SerializerType>(serializer)
        );
    }

    void paint()
    {
        holder->paint();
    }

    std::string serialize()
    {
        return holder->serialize();
    }

private:
    std::unique_ptr<ShapeHolder> holder;
};


TEST_CASE("Type erasure example", "[TEE]")
{
    Shape circle{Circle{1.5}, CirclePainter{}, CircleSerializer{}};
    Shape boldCircle{Circle{2}, CircleBoldPainter{}, CircleBinarySerializer{}};
    Shape rect{Rectangle{1, 2}, RectanglePainter{}, RectangleSerializer{}};

    std::cout << "Paint shapes" << std::endl;
    circle.paint();
    boldCircle.paint();
    rect.paint();

    std::cout << std::endl;

    std::cout << "Serialize shapes" << std::endl;
    std::cout << "Circle serialization: " << circle.serialize() << std::endl;
    std::cout << "Bold circle serialization: " << boldCircle.serialize() << std::endl;
    std::cout << "Rect serialization: " << rect.serialize() << std::endl;
}
