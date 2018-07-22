#pragma once

template <typename T>
struct property final {
private:
    template<typename R>
    struct accessor;

    template<typename R, typename... U>
    struct accessor <R(U...)> {
    private:
        struct Function {
            virtual R invoke(U... args) = 0;
        };

        template <typename M>
        struct FunctionObject : Function {
            M function;

            inline FunctionObject(const M &function) : function(function) {}

            inline virtual R invoke(U... args) override {
                return function(args...);
            }
        };

        Function *function;

    public:
        inline accessor() : function(0) {}

        template <typename M>
        inline accessor(const M &function) : function(new FunctionObject<M>(function)) {}

        inline ~accessor() {
            if (function) {
                //delete function;
                function = nullptr;
            }
        }

        inline R operator()(U... args) const {
            return function->invoke(args...);
        }
    };

    using Getter = accessor<T()>;
    using Setter = accessor<void(const T&)>;

    Getter getter;
    Setter setter;

public:
    inline property(const Getter& getter, const Setter& setter) : getter(getter), setter(setter) {}

    inline property<T>& operator=(const T& value) {
        setter(value);
        return *this;
    }

    inline operator T() {
        return getter();
    }

    inline operator const T() const {
        return getter();
    }
};