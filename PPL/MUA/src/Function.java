import com.sun.org.apache.xpath.internal.operations.Bool;

import java.lang.RuntimeException;
import java.util.function.IntBinaryOperator;
import java.util.function.DoubleBinaryOperator;
public class Function
{
    static WordList wordList;

    static Value thing(WordStream wordStream) throws RunningException, SyntaxException
    {
        String name = wordStream.next();
        return wordList.thing(name);
    }

    static Value isName(WordStream wordStream)
    {
        String name = wordStream.next();
        return wordList.isname(name);
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

    static Value numericOperate(WordStream stream, IntBinaryOperator intBinaryOperator, DoubleBinaryOperator doubleBinaryOperator) throws RunningException
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

    static Value numericCompare(WordStream stream, IntBinaryComparator intBinaryComparator, DoubleBinaryComparator doubleBinaryComparator) throws RunningException
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
                throw new RuntimeException("One of the operand cannot convert to numeric.");
            }
        } catch (RuntimeException e)
        {
            throw e;
        }
    }

    static Value booleanOperate(WordStream stream, BooleanBinaryOperator booleanBinaryOperator) throws RunningException
    {
        Value first = Interpreter.value(stream);
        Value second = Interpreter.value(stream);
        Boolean a = first.toBoolean();
        Boolean b = second.toBoolean();
        return new Value(String.valueOf(booleanBinaryOperator.apply(a, b)));
    }

    static Value add(WordStream stream) throws RunningException
    {
        return numericOperate(stream, (int a, int b) -> a + b, (double a, double b) -> a + b);
    }

    static Value sub(WordStream stream) throws RunningException
    {
        return numericOperate(stream, (int a, int b) -> a - b, (double a, double b) -> a - b);
    }

    static Value mul(WordStream stream) throws RunningException
    {
        return numericOperate(stream, (int a, int b) -> a * b, (double a, double b) -> a * b);
    }

    static Value div(WordStream stream) throws RunningException
    {
        return numericOperate(stream, (int a, int b) -> a / b, (double a, double b) -> a / b);
    }

    static Value mod(WordStream stream) throws RunningException
    {
        return numericOperate(stream, (int a, int b) -> a % b, (double a, double b) -> a % b);
    }

    static Value eq(WordStream stream) throws RunningException
    {
        return numericCompare(stream, Integer::equals, Double::equals);
    }

    static Value lt(WordStream stream) throws RunningException
    {
        return numericCompare(stream, (Integer a, Integer b) -> a.compareTo(b) < 0, (Double a, Double b) -> a.compareTo(b) < 0);
    }

    static Value gt(WordStream stream) throws RunningException
    {
        return numericCompare(stream, (Integer a, Integer b) -> a.compareTo(b) > 0, (Double a, Double b) -> a.compareTo(b) > 0);
    }

    static Value and(WordStream stream) throws RunningException
    {
        return booleanOperate(stream, Boolean::logicalAnd);
    }

    static Value or(WordStream stream) throws RunningException
    {
        return booleanOperate(stream, Boolean::logicalOr);
    }

    static Value not(WordStream stream) throws RunningException
    {
        Value first = Interpreter.value(stream);
        Boolean a = first.toBoolean();
        return new Value(String.valueOf(!a));
    }

    static void make(WordStream stream)
    {

    }

    static void erase(WordStream stream)
    {

    }

    static void print(WordStream stream)
    {

    }
}
