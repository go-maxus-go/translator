#include <iostream>

#include <catch.hpp>


struct Circle
{
    double radius = 0;
};

struct Rectangle
{
    double width = 0;
    double height = 0;
};

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

struct CircleSerializer
{
    std::vector<std::byte> serialize(const Circle&)
    {
        return std::vector<std::byte>{0};
    }
};

struct RectangleSerializer
{
    std::vector<std::byte> serialize(const Rectangle&)
    {
        return std::vector<std::byte>{1};
    }
};

struct RectangleBinarySerializer
{
    std::vector<std::byte> serialize(const Rectangle&)
    {
        return std::vector<std::byte>{2};
    }
};

class Shape
{
    struct ShapeHolder
    {
        virtual ~ShapeHolder() = default;

        virtual void paint() = 0;
        virtual std::vector<std::byte> serialize() = 0;
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

        std::vector<std::byte> serialize() override
        {
            return serializer.serialize(shape);
        }

        ShapeType shape;
        PainterType painter;
        SerializerType serializer;
    };

public:
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

    std::vector<std::byte> serialize()
    {
        return holder->serialize();
    }

private:
    std::unique_ptr<ShapeHolder> holder;
};


TEST_CASE("Trial of type erasure")
{
    Shape circle{Circle{1.5}, CirclePainter{}, CircleSerializer{}};
    Shape boldCircle{Circle{2}, CircleBoldPainter{}, CircleSerializer{}};
    Shape rect{Rectangle{1, 2}, RectanglePainter{}, RectangleBinarySerializer{}};

    circle.paint();
    boldCircle.paint();
    rect.paint();
    rect.serialize();
}
