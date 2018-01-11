import java.io.Serializable;
import java.math.BigDecimal;
import java.math.BigInteger;

abstract class Value implements Iterable<Value>, Serializable {
    @SuppressWarnings("StaticInitializerReferencesSubClass")
    static final Value ZERO = new Word("0");
    @SuppressWarnings("StaticInitializerReferencesSubClass")
    static final Value TRUE = new Word("true");
    @SuppressWarnings("StaticInitializerReferencesSubClass")
    static final Value FALSE = new Word("false");

    abstract WordStream toWordStream();

    abstract boolean isList();

    abstract boolean isBool();

    abstract boolean isEmpty();

    abstract boolean isNumber();

    abstract int size();

    abstract BigDecimal toNumeric()
            throws RunningException;

    abstract boolean toBoolean()
            throws RunningException;

    abstract Value item(BigInteger i)
            throws RunningException;

    abstract Value item(int i);

    abstract Value but(int i);

    abstract BigInteger toInt()
            throws RunningException;
}

