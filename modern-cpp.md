### How does `is_base_of` work?
```cpp
typedef char (&yes)[1];
typedef char (&no)[2];

template <typename B, typename D>
struct Host
{
  operator B*() const;
  operator D*();
};

template <typename B, typename D>
struct is_base_of
{
  template <typename T> 
  static yes check(D*, T);
  static no check(B*, int);

  static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(yes);
};

//Test sample
class Base {};
class Derived : private Base {};

//Expression is true.
int test[is_base_of<Base,Derived>::value && !is_base_of<Derived,Base>::value];
```
```


Let's work out how it works by looking at the steps.

Start with the sizeof(check(Host<B,D>(), int())) part. The compiler can quickly see that this check(...) 
is a function call expression, so it needs to do overload resolution on check. There are two candidate 
overloads available, template <typename T> yes check(D*, T); 
and no check(B*, int);. If the first is chosen, you get sizeof(yes), else sizeof(no)

Next, let's look at the overload resolution. The first overload is a template instantiation 
check<int> (D*, T=int) and the second candidate is check(B*, int). The actual arguments provided 
are Host<B,D> and int(). The second parameter clearly doesn't distinguish them; 
it merely served to make the first overload a template one. 
We'll see later why the template part is relevant.

Now look at the conversion sequences that are needed. For the first overload, 
we have Host<B,D>::operator D* - one user-defined conversion. 
For the second, the overload is trickier. We need a B*, but there are possibly two conversion sequences. 
One is via Host<B,D>::operator B*() const. 
If (and only if) B and D are related by inheritance will the conversion sequence 
Host<B,D>::operator D*() + D*->B* exist. Now assume D indeed inherits from B. The two conversion sequences are 
Host<B,D> -> Host<B,D> const -> operator B* const -> B* ,and 
Host<B,D> -> operator D* -> D* -> B*.

So, for related B and D, no check(<Host<B,D>(), int()) would ambiguous. As a result, 
the templated yes check<int>(D*, int) is chosen. However, if D does not inherit from B, 
then no check(<Host<B,D>(), int()) is not ambiguous. At this point, overload resolution cannot happen 
baed on shortest conversion sequence. However, given equal conversion sequences, 
overload resolution prefers non-template functions, i.e. no check(B*, int).

You now see why it doesn't matter that the inheritance is private: 
that relation only serves to eliminate no check(Host<B,D>(), int()) from overload resolution 
before the access check happens. And you also see why the operator B* const must be const: 
else there's no need for the Host<B,D> -> Host<B,D> const step, no ambiguity, and no check(B*, int) would always be chosen.

```
```
If they are related

Let's for a moment assume that B is actually a base of D. Then for the call to check, 
both versions are viable because Host can be converted to D* and B*. 
It's a user defined conversion sequence as described by 13.3.3.1.2 from Host<B, D> 
to D* and B* respectively. For finding conversion functions that can convert the class, 
the following candidate functions are synthesized for the first check function according to 13.3.1.5/1

D* (Host<B, D>&)

The first conversion function isn't a candidate, because B* can't be converted to D*.

For the second function, the following candidates exist:

B* (Host<B, D> const&)
D* (Host<B, D>&)

Those are the two conversion function candidates that take the host object. 
The first takes it by const reference, and the second doesn't. 
Thus the second is a better match for the non-const *this object 
(the implied object argument) by 13.3.3.2/3b1sb4 and is used to convert to B* 
for the second check function.

If you would remove the const, we would have the following candidates

B* (Host<B, D>&)
D* (Host<B, D>&)

This would mean that we can't select by constness anymore. 
In an ordinary overload resolution scenario, the call would now be ambiguous 
because normally the return type won't participate in overload resolution. 
For conversion functions, however, there is a backdoor. 
If two conversion functions are equally good, 
then the return type of them decides who is best according to 13.3.3/1. 
Thus, if you would remove the const, then the first would be taken, 
because B* converts better to B* than D* to B*.

Now what user defined conversion sequence is better? 
The one for the second or the first check function? 
The rule is that user defined conversion sequences can only be compared 
if they use the same conversion function or constructor according to 13.3.3.2/3b2. 
This is exactly the case here: Both use the second conversion function. 
Notice that thus the const is important because it forces the compiler to 
take the second conversion function.

Since we can compare them - which one is better? 
The rule is that the better conversion from the return type of the conversion function 
to the destination type wins (again by 13.3.3.2/3b2). 
In this case, D* converts better to D* than to B*. 
Thus the first function is selected and we recognize the inheritance!

Notice that since we never needed to actually convert to a base class, 
we can thereby recognize private inheritance because whether we can convert 
from a D* to a B* isn't dependent on the form of inheritance according to 4.10/3
If they are not related

Now let's assume they are not related by inheritance. 
Thus for the first function we have the following candidates

D* (Host<B, D>&) 

And for the second we now have another set

B* (Host<B, D> const&)

Since we cannot convert D* to B* if we haven't got a inheritance relationship, 
we now have no common conversion function among the two user defined conversion sequences! 
Thus, we would be ambiguous if not for the fact that the first function is a template. 
Templates are second choice when there is a non-template function that is equally 
good according to 13.3.3/1. Thus, we select the non-template function (second one) 
and we recognize that there is no inheritance between B and D!
```


### how does is_pointer work?

```cpp
template <typename T>
struct remove_const
{
    typedef T type;
};

template <typename T>
struct remove_const<const T>
{
    typedef T type;
};

template <typename T>
struct remove_volatile
{
    typedef T type;
};

template <typename T>
struct remove_volatile<volatile T>
{
    typedef T type;
};

template <typename T>
struct remove_cv : remove_const<typename remove_volatile<T>::type> {};

template <typename T>
struct is_unqualified_pointer
{
    enum { value = false };
};

template <typename T>
struct is_unqualified_pointer<T*>
{
    enum { value = true };
};

template <typename T>
struct is_pointer_type : is_unqualified_pointer<typename remove_cv<T>::type> {};

template <typename T>
bool is_pointer(const T&)
{
    return is_pointer_type<T>::value;
}
```
