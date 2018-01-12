import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.StringJoiner;

class List extends Value {
    private ArrayList<Value> value;

    private List() {
        value = new ArrayList<>();
    }

    List(WordStream stream) {
        value = new ArrayList<>();
        mainLoop:
        while (true) {
            String first = stream.next(true);
            switch (first) {
                case "[":
                    value.add(new List(stream));
                    break;
                case "]":
                    break mainLoop;
                default:
                    value.add(new Word(first));
                    break;
            }
        }
    }

    List(String[] list) {
        for (String s : list)
            value.add(new Word(s));
    }

    List(Value[] list) {
        this.value.addAll(Arrays.asList(list));
    }

    static List list(Value first, Value second) {
        List value = new List();
        value.value.add(first);
        value.value.add(second);
        return value;
    }

    static List join(Value first, Value second) {
        List value = new List();
        for (Value v : first)
            value.value.add(v);
        value.value.add(second);
        return value;
    }

    static List sentence(Value first, Value second) {
        List value = new List();
        for (Value v : first)
            value.value.add(v);
        for (Value v : second)
            value.value.add(v);
        return value;
    }

    @Override
    WordStream toWordStream() {
        WordStream stream = new WordStream(false);
        for (Value v : value) {
            if (v.isList())
                stream.merge("[");
            stream.merge(v.toWordStream());
            if (v.isList())
                stream.merge("]");
        }
        return stream;
    }

    @Override
    boolean isList() {
        return true;
    }

    @Override
    boolean isBool() {
        return false;
    }

    @Override
    boolean isEmpty() {
        return value.isEmpty();
    }

    @Override
    boolean isNumber() {
        return false;
    }

    @Override
    int size() {
        return value.size();
    }

    @Override
    BigDecimal toNumeric()
            throws RunningException {
        if (size() == 1 && !value.get(0).isList()) {
            return new BigDecimal(value.get(0).toString());
        } else
            throw new RunningException("List cannot be converted to numeric");

    }

    @Override
    boolean toBoolean()
            throws RunningException {
        if (size() == 1 && !value.get(0).isList()) {
            return value.get(0).toBoolean();
        } else
            throw new RunningException("List cannot be converted to boolean");
    }

    @Override
    Value item(BigInteger i)
            throws RunningException {
        if (value.size() > 0 && i.compareTo(BigInteger.valueOf(value.size())) < 0)
            return item(i.intValue());
        else
            throw new RunningException(i + " out of bound.");
    }

    @Override
    Value item(int i) {
        if (i < value.size())
            return value.get(i);
        else
            return null;
    }

    @Override
    Value but(int i) {
        List list = new List();
        for (int j = 0; j < value.size(); ++j)
            if (i != j)
                list.value.add(value.get(j));

        return list;
    }

    @Override
    BigInteger toInt()
            throws RunningException {
        if (size() == 1 && !value.get(0).isList()) {
            return value.get(0).toInt();
        } else
            throw new RunningException("List cannot be converted to int");
    }

    @Override
    public Iterator<Value> iterator() {
        return value.iterator();
    }

    @Override
    public String toString() {
        StringJoiner stringJoiner = new StringJoiner(" ");
        for (Value v : this) {
            if (v.isList())
                stringJoiner.add("[");
            stringJoiner.add(v.toString());
            if (v.isList())
                stringJoiner.add("]");
        }
        return stringJoiner.toString();
    }
}
