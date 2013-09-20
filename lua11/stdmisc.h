#ifndef _STD_MISC_H__
#define _STD_MISC_H__

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

namespace std
{
	class misc
	{
	public:
		template<typename TIn, typename TOut> static bool cast(const TIn& input, TOut& output)
		{
			stringstream stream;
			stream << input;
			stream >> output;
			return !stream.fail();
		}

		template<typename TIn, typename TOut> static TOut cast(const TIn& input, const TOut& defval)
		{
			TOut output;
			return cast(input, output) ? output : defval;
		}

		template<class... Args, class Ret> static Ret forward(tuple<Args...> tuple, Ret (*fptr) (Args...))
		{
			typedef typename make_indices<Args...>::type Indices;
			return forward_impl(Indices(), tuple, fptr);
		}

		template<class... Args, class Ret> static Ret forward(tuple<Args...> tuple, function<Ret(Args...)> func)
		{
			typedef typename make_indices<Args...>::type Indices;
			return forward_impl(Indices(), tuple, func);
		}

		template<class... Args, class Ret> static Ret forwardref(tuple<Args...>& tuple, Ret (*fptr) (Args&...))
		{
			typedef typename make_indices<Args...>::type Indices;
			return forwardref_impl(Indices(), tuple, fptr);
		}

		template<class... Args, class Ret> static Ret forwardref(tuple<Args...>& tuple, function<Ret(Args&...)> func)
		{
			typedef typename make_indices<Args...>::type Indices;
			return forwardref_impl(Indices(), tuple, func);
		}

#	if GCC_VERSION >= 30700
		template<typename T> struct remove_class { };
		template<typename C, typename R, typename ...A> struct remove_class<R(C::*)(A...)> { using type = R(A...); };
		template<typename C, typename R, typename ...A> struct remove_class<R(C::*)(A...) const> { using type = R(A...); };
		template<typename C, typename R, typename ...A> struct remove_class<R(C::*)(A...) volatile> { using type = R(A...); };
		template<typename C, typename R, typename ...A> struct remove_class<R(C::*)(A...) const volatile> { using type = R(A...); };

		template<typename T> struct get_signature_impl { using type = typename remove_class<decltype(&std::remove_reference<T>::type::operator())>::type; };
		template<typename R, typename ...A> struct get_signature_impl<R(A...)> { using type = R(A...); };
		template<typename R, typename ...A> struct get_signature_impl<R(&)(A...)> { using type = R(A...); };
		template<typename R, typename ...A> struct get_signature_impl<R(*)(A...)> { using type = R(A...); };
		template<typename T> using get_signature = typename get_signature_impl<T>::type;

		template<typename F> using make_function_type = std::function<get_signature<F>>;
		template<typename F> static make_function_type<F> make_function(F &&f) { return make_function_type<F>(std::forward<F>(f)); }
#	endif

	private:
		template<unsigned...> struct index_tuple { };

		template<unsigned I, typename IndexTuple, typename... Types> struct make_indices_impl;

		template<unsigned I, unsigned... Indices, typename T, typename... Types> struct make_indices_impl<I, index_tuple<Indices...>, T, Types...>
		{
			typedef typename make_indices_impl<I + 1, index_tuple<Indices..., I>, Types...>::type type;
		};

		template<unsigned I, unsigned... Indices> struct make_indices_impl<I, index_tuple<Indices...> >
		{
			typedef index_tuple<Indices...> type;
		};

		template<typename... Types> struct make_indices : make_indices_impl<0, index_tuple<>, Types...> { };

		template <unsigned... Indices, class... Args, class Ret> static Ret forward_impl(index_tuple<Indices...>, tuple<Args...> tuple, Ret (*fptr) (Args...))
		{
			return fptr(get<Indices>(tuple)...);
		}

		template <unsigned... Indices, class... Args, class Ret> static Ret forward_impl(index_tuple<Indices...>, tuple<Args...> tuple, function<Ret(Args...)> func)
		{
			return func(get<Indices>(tuple)...);
		}

		template <unsigned... Indices, class... Args, class Ret> static Ret forwardref_impl(index_tuple<Indices...>, tuple<Args...>& tuple, Ret (*fptr) (Args&...))
		{
			return fptr(get<Indices>(tuple)...);
		}

		template <unsigned... Indices, class... Args, class Ret> static Ret forwardref_impl(index_tuple<Indices...>, tuple<Args...>& tuple, function<Ret(Args&...)> func)
		{
			return func(get<Indices>(tuple)...);
		}
	};
}

#endif // _STD_MISC_H__
