#include <string>

#include <catch.hpp>

#include <fakeit.hpp>
#include <mockitopp/mockitopp.hpp> // path to <mockitopp.hpp> doesn't work
#include <mockitopp/matchers.hpp>


using namespace fakeit;
using namespace mockitopp;

TEST_CASE("A simple interface can be mocked and verified")
{
    struct SimpleInterface
    {
        virtual ~SimpleInterface() = default;

        virtual void voidFunction() = 0;

        virtual void voidFunctionWithParameters(int, std::string) = 0;

        virtual void overload(int) = 0;
        virtual void overload(float) = 0;

        virtual void constOverload() = 0;
        virtual void constOverload() const = 0;

        virtual int returnFunction() const = 0;
    };

    Mock<SimpleInterface> fakeItMock;
    mock_object<SimpleInterface> mockitoppMock;

    SECTION("A void function without input parameters can be mocked and verified")
    {
        // FakeIt has better error messages
        Fake(Method(fakeItMock, voidFunction));
        mockitoppMock(&SimpleInterface::voidFunction).when().thenReturn().exactly(1);

        auto& fakeItObject = fakeItMock.get();
        auto& mockitoppObject = mockitoppMock.getInstance();

        fakeItObject.voidFunction();
        mockitoppObject.voidFunction();

        Verify(Method(fakeItMock, voidFunction)).Once();
        // This way from the documentation doesn't compile
        // mockitoppMock.verify(&SimpleInterface::voidFunction).exactly(1);
    }

    SECTION("A void function with input parameters can be mocked and verified")
    {
        Fake(Method(fakeItMock, voidFunctionWithParameters).Using(0, ""));
        mockitoppMock(&SimpleInterface::voidFunctionWithParameters).when(0, "")
            .thenReturn().exactly(1);

        auto& fakeItObject = fakeItMock.get();
        auto& mockitoppObject = mockitoppMock.getInstance();

        fakeItObject.voidFunctionWithParameters(0, "");
        mockitoppObject.voidFunctionWithParameters(0, "");

        Verify(Method(fakeItMock, voidFunctionWithParameters)).Once();
    }

    SECTION("An overloaded function can be mocked and verified")
    {
        Fake(OverloadedMethod(fakeItMock, overload, void(int)).Using(0));
        mockitoppMock(static_cast<void(SimpleInterface::*)(int)>(&SimpleInterface::overload))
            .when(0).thenReturn().exactly(1);

        auto& fakeItObject = fakeItMock.get();
        auto& mockitoppObject = mockitoppMock.getInstance();

        fakeItObject.overload(0);
        mockitoppObject.overload(0);

        Verify(OverloadedMethod(fakeItMock, overload, void(int))).Once();
    }

    SECTION("A const overloaded function can be mocked and verified")
    {
        Fake(ConstOverloadedMethod(fakeItMock, constOverload, void()));
        mockitoppMock(static_cast<void(SimpleInterface::*)(void)const>(&SimpleInterface::constOverload))
            .when().thenReturn().exactly(1);

        auto& fakeItObject = fakeItMock.get();
        auto& mockitoppObject = mockitoppMock.getInstance();

        std::as_const(fakeItObject).constOverload();
        std::as_const(mockitoppObject).constOverload();

        Verify(ConstOverloadedMethod(fakeItMock, constOverload, void())).Once();
    }

    SECTION("A function with a return parameter can be mocked and verified")
    {
        When(Method(fakeItMock, returnFunction)).AlwaysReturn(42);
        mockitoppMock(&SimpleInterface::returnFunction).when().thenReturn(42).exactly(1);

        auto& fakeItObject = fakeItMock.get();
        auto& mockitoppObject = mockitoppMock.getInstance();

        REQUIRE(fakeItObject.returnFunction() == 42);
        REQUIRE(mockitoppObject.returnFunction() == 42);
    }
}


template<class IdType>
struct BaseInterface
{
    virtual ~BaseInterface() = default;

    virtual IdType id() = 0;
};

TEST_CASE("A single nested interface can be mocked and verified")
{
    struct NastyInterface : BaseInterface<int>
    {
        virtual ~NastyInterface() = default;

        virtual void verifyId() = 0;
    };

    Mock<NastyInterface> fakeItMock;
    mock_object<NastyInterface> mockitoppMock;

    SECTION("A base interface function can be mocked and verified")
    {
        When(Method(fakeItMock, id)).AlwaysReturn(42);
        mockitoppMock(&NastyInterface::id).when().thenReturn(42).exactly(1);

        auto& fakeItObject = fakeItMock.get();
        auto& mockitoppObject = mockitoppMock.getInstance();

        REQUIRE(fakeItObject.id() == 42);
        REQUIRE(mockitoppObject.id() == 42);
        Verify(Method(fakeItMock, id)).Once();
    }

    SECTION("A child interface function can be mocked and verified")
    {
        Fake(Method(fakeItMock, verifyId));
        mockitoppMock(&NastyInterface::verifyId).when().thenReturn().exactly(1);

        auto& fakeItObject = fakeItMock.get();
        auto& mockitoppObject = mockitoppMock.getInstance();

        fakeItObject.verifyId();
        mockitoppObject.verifyId();

        Verify(Method(fakeItMock, verifyId)).Once();
    }
}

TEST_CASE("An interface inherited from a concrete type can be mocked and verified")
{
    struct BaseClass
    {
        std::string name() const { return "name"; }
    };

    struct Interface : BaseClass
    {
        virtual ~Interface() = default;

        virtual void verifyId() = 0;
    };

    Mock<Interface> fakeItMock;

    // According to the documentation it is not not stable
    mock_object<Interface> mockitoppMock;

    Fake(Method(fakeItMock, verifyId));
    mockitoppMock(&Interface::verifyId).when().thenReturn().exactly(1);

    auto& fakeItObject = fakeItMock.get();
    auto& mockitoppObject = mockitoppMock.getInstance();

    fakeItObject.verifyId();
    mockitoppObject.verifyId();

    Verify(Method(fakeItMock, verifyId)).Once();
}

TEST_CASE("An interface inherited from several interfaces can be mocked and verified")
{
    struct Disposable
    {
        virtual ~Disposable() = default;
    };

    struct Interface : BaseInterface<float>, Disposable
    {
        virtual ~Interface() = default;

        virtual void verifyId() = 0;
    };

    // FakeIt doesn't support it.
//    Mock<Interface> fakeItMock;

    mock_object<Interface> mockitoppMock;

//    Fake(Method(fakeItMock, verifyId));
    mockitoppMock(&Interface::verifyId).when().thenReturn().exactly(1);

//    auto& fakeItObject = fakeItMock.get();
    auto& mockitoppObject = mockitoppMock.getInstance();

//    fakeItObject.verifyId();
    mockitoppObject.verifyId();

//    Verify(Method(fakeItMock, verifyId)).Once();
}
