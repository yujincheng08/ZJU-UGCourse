# Java Assignment 3
|余锦成|3150101155|
|:-:|:-:|
对`String`、`StringBuilder`以及`StringBuffer`进行源代码分析。
1. 分析其主要数据组织及功能实现，有什么区别？
    `StringBuilder`和`StringBuffer`均继承自`AbstractStringBuilder`，其中，`StringBuilder`几乎重载所有的方法，而`StringBuffer`也继承了大部分的方法。这是因为`StringBuilder`是线程安全的，它要重载几乎所有父类函数才能在方法定义时候加入`synchronized`。两者大部分的override都是直接调用父类的方法。其中在jdk8后，`StringBuilder`还实现了一个`toStringCache`用于多次`toString`时候可以提高效率，因为它的线程安全对速度有一定的影响，用于弥补。
    `AbstractStringBuilder`和`String`均有一个`char[] value`的成员数据。不同点在于，`String`中的`value`是`private final`形式，而`AbstractStringBuilder`的`value`没有修饰符，即它在`java.lang`包中是可见的，应该是便于标准包内更加轻松使用`StringBuilder`和`StringBuffer`。
    但是，在`AbstractStringBuilder`中，有一个`int count`的成员数据，而`String`中并没有。在`AbstractStringBuilder`中，`int length()`返回的是`count`，而在`String`中直接返回`value.length`。`String`之所以可以直接返回`value.length`是因为它的长度正是这个不会再变化的`value`字符数组的长度，而`AbstractStringBuilder`中，`count`的长度才是字符串的长度，`value.length`有可能会比`count`要大。这是因为`AbstractStringBuilder`中的数组长度变化出于效率考虑，跟c++采用一样的效率，是进行二倍增长的。
    ```java
    void expandCapacity(int minimumCapacity) {
        int newCapacity = value.length * 2 + 2;
        if (newCapacity - minimumCapacity < 0)
            newCapacity = minimumCapacity;
        if (newCapacity < 0) {
            if (minimumCapacity < 0) // overflow
                throw new OutOfMemoryError();
            newCapacity = Integer.MAX_VALUE;
        }
        value = Arrays.copyOf(value, newCapacity);
    }
    ```
    对于功能方法的实现，两者区别非常大。对于`AbstractStringBuilder`及其子类而言，功能着重于修改字符串：主要是`append`、`insert`等功能。而对于`String`而言，主要在于对字符串非修改性的操作：`compareTo`、`indexOf`、`startsWith`、`equals`等。当然两者也有一些共同操作，如`CharAt`、`substring`、`lastIndexOf`等，但是并不是很多。正如其名，`AbstractStringBuilder`及其子类着重于构建出一个`String`，而构建出来的`String`才是用于应用的。
2. 说明为什么这样设计，这么设计对`String`、`StringBuilder`及`StringBuffer`的影响？
    需要把`String`和`StringBuilder`或`StringBuffer`等构造`String`的类分离的主要原因是`String`是不可变类。所以在循环或者需要把不同字符串连接起来的时候，使用`String`相加会有非常高的代价，因为每次都创建了一个`String`，而且`String`是按需增长长度的，不是以两倍的方式增长长度，所以效率会更加地下。而且其实在实际上，`String`的连接也是靠`StringBuilder`实现的。
    所以关键就在于`String`为什么是不可变的。如果`String`可变的，就没有`AbstractStringBuilder`及其子类存在了。
    可以从以下几个方面考虑`String`为什么是不可变类。
    - 函数传参：
        由于`String`是偏向应用方面的，所以很多函数使用`String`进行传参，由于在Java中，函数对对象传参是传引用的，所以我在传参之后就不能保证这个`String`依然是我调用之前的内容。这样很容易引起奇怪的bug。
    - 线程安全
        由于`String`是不可变类，所以可以保证它的线程安全，不会有线程同步的问题。如果它是可变类，则程序员在使用`String`时候还需要线程同步。
    - 缓存
        `String`是可以进行缓存的：如果两个`String`的内容是一样的话，它会指向同一个对象。如果`String`是可变的话，就不能进行字符串的缓存了。因为如果两个在某个时候拥有相同内容的`String`指向了同一个对象，在修改一个对象内容的时候，要保证另一个不被修改，只能对对象进行复制。所以缓存在不可变类的实现不怎么可能。
    正是由于以上几点原因（可能有纰漏），在Java中，`String`被设计为不可变类。为了可以让程序要构造一个`String`，于是就有了`StringBuilder`和`StringBuffer`了。正如上一节所说，还有一个线程安全的问题。所以需要对应一个线程安全的`StringBuilder`和线程不安全的`StringBuffer`。在不需要同步线程的时候，我们就可以选择使用`StringBuffer`以提高效率。另外在默认情况下，编译器是使用`StringBuilder`的。
    于是影响可以这样概括：`String`是线程安全的，也是传参安全的；`StringBuilder`是线程安全的，但是传参不安全；`StringBuffer`即不线程安全，也不传参安全。
3. `String`、`StringBuilder`及`StringBuffer`分别适合哪些场景？
    通过上面的总结，我们可以在得出它们分别的应用场景：
    - `String`：
        在不需要频繁改变值的时候，使用`String`，因为比较安全，不需要考虑很多的情况。
    - `StringBuilder`：
        在构建`String`时候，或者需要频繁改变字符串内容的时候使用。并且需要在用到线程安全的时候使用。
    - `StringBuffer`：
        在不使用多线程编程或者不需要考虑线程安全的时候，可以用于替代`StringBuilder`以获得性能的提高。
