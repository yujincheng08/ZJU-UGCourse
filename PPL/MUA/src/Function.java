import java.lang.RuntimeException;
import java.util.function.IntBinaryOperator;
import java.util.function.DoubleBinaryOperator;
public class Function
{
    static WordList wordList = new WordList();

    private static String getWordName(WordStream stream)
            throws SyntaxException, RunningException
    {
        Value value = Interpreter.value(stream);
        if(value.size()!=1)
            throw new SyntaxException("Except a word literary");
        else
            return value.toString();

        /*
        String name = Interpreter.value(stream).toString();
        if(Interpreter.isWordLiterary(name))
            return name.substring(1);
        else
            throw new RunningException("Except a word literary");
            */
    }

    static Value thing(WordStream stream)
            throws RunningException, SyntaxException
    {
        return thing(getWordName(stream));
    }

    static Value thing(String name)
            throws RunningException, SyntaxException
    {
        return wordList.thing(name);
    }

    static Value isName(WordStream stream)
            throws RunningException, SyntaxException
    {
        return wordList.isname(getWordName(stream));
    }

    static Value read()
    {
        //TODO
        return new Value();
    }

    static Value readLinst()
    {
        //TODO
        return new Value();
    }

    static Value numericOperate(WordStream stream, IntBinaryOperator intBinaryOperator, DoubleBinaryOperator doubleBinaryOperator)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream);
        Value second = Interpreter.value(stream);
        try
        {
            int a = first.toInt();
            int b = second.toInt();
            return new Value(String.valueOf(intBinaryOperator.applyAsInt(a, b)));
        } catch (NumberFormatException e)
        {
            try
            {
                double a = first.toFloat();
                double b = second.toFloat();
                return new Value(String.valueOf(doubleBinaryOperator.applyAsDouble(a, b)));
            } catch (NumberFormatException f)
            {
                throw new RuntimeException("One of the operand cannot convert to numeric.");
            }
        } catch (RuntimeException e)
        {
            throw e;
        }
    }

    static Value compare(WordStream stream, IntBinaryComparator intBinaryComparator, DoubleBinaryComparator doubleBinaryComparator, StringBinaryComparator stringBinaryComparator)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream);
        Value second = Interpreter.value(stream);
        try
        {
            int a = first.toInt();
            int b = second.toInt();
            return new Value(String.valueOf(intBinaryComparator.apply(a, b)));
        } catch (NumberFormatException e)
        {
            try
            {
                double a = first.toFloat();
                double b = second.toFloat();
                return new Value(String.valueOf(doubleBinaryComparator.apply(a, b)));
            } catch (NumberFormatException f)
            {
                return new Value(String.valueOf(stringBinaryComparator.apply(first.toString(), second.toString())));
            }
        } catch (RuntimeException e)
        {
            throw e;
        }
    }

    static Value booleanOperate(WordStream stream, BooleanBinaryOperator booleanBinaryOperator)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream);
        Value second = Interpreter.value(stream);
        Boolean a = first.toBoolean();
        Boolean b = second.toBoolean();
        return new Value(String.valueOf(booleanBinaryOperator.apply(a, b)));
    }

    static Value add(WordStream stream)
            throws RunningException, SyntaxException
    {
        return numericOperate(stream, (int a, int b) -> a + b, (double a, double b) -> a + b);
    }

    static Value sub(WordStream stream)
            throws RunningException, SyntaxException
    {
        return numericOperate(stream, (int a, int b) -> a - b, (double a, double b) -> a - b);
    }

    static Value mul(WordStream stream)
            throws RunningException, SyntaxException
    {
        return numericOperate(stream, (int a, int b) -> a * b, (double a, double b) -> a * b);
    }

    static Value div(WordStream stream)
            throws RunningException, SyntaxException
    {
        try
        {
            return numericOperate(stream, (int a, int b) -> a / b, (double a, double b) -> a / b);
        }catch(ArithmeticException e)
        {
            throw new RunningException("Cannot divided by zero");
        }

    }

    static Value mod(WordStream stream)
            throws RunningException, SyntaxException
    {
        return numericOperate(stream, (int a, int b) -> a % b, (double a, double b) -> a % b);
    }

    static Value eq(WordStream stream)
            throws RunningException, SyntaxException
    {
        return compare(stream, Integer::equals, Double::equals, String::equals);
    }

    static Value lt(WordStream stream)
            throws RunningException, SyntaxException
    {
        return compare(stream, (Integer a, Integer b) -> a.compareTo(b) < 0, (Double a, Double b) -> a.compareTo(b) < 0, (String a, String b) -> a.compareTo(b) < 0);
    }

    static Value gt(WordStream stream)
            throws RunningException, SyntaxException
    {
        return compare(stream, (Integer a, Integer b) -> a.compareTo(b) > 0, (Double a, Double b) -> a.compareTo(b) > 0, (String a, String b) -> a.compareTo(b) > 0);
    }

    static Value and(WordStream stream)
            throws RunningException, SyntaxException
    {
        return booleanOperate(stream, Boolean::logicalAnd);
    }

    static Value or(WordStream stream)
            throws RunningException, SyntaxException
    {
        return booleanOperate(stream, Boolean::logicalOr);
    }

    static Value not(WordStream stream)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream);
        Boolean a = first.toBoolean();
        return new Value(String.valueOf(!a));
    }

    static void make(WordStream stream)
            throws RunningException, SyntaxException
    {
        wordList.make(getWordName(stream), stream);
    }

    static void erase(WordStream stream)
            throws RunningException, SyntaxException
    {
        wordList.erase(getWordName(stream));
    }

    static void print(WordStream stream)
            throws RunningException, SyntaxException
    {
        System.out.println(Interpreter.value(stream).toString());
    }
}
