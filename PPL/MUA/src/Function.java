import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigDecimal;

class Function
{
    private static WordList wordList = new WordList();

    private static String getWordName(WordStream stream)
            throws SyntaxException, RunningException
    {
        Value value = Interpreter.value(stream);
        String name = value.toString();
        if(value.size()!=1 || !name.matches("[_a-zA-Z].*"))
            throw new SyntaxException("Except a word literary but get "+name);
        else
            return name;

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

    static Value read(WordStream stream)
    {
        System.out.print("Input a word: ");
        WordStream tmp = new WordStream();
        String word = tmp.next();
        stream.merge(tmp);
        return new Value(word);
    }

    static Value readLinst()
    {
        System.out.print("Input a list: > ");
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));
        try
        {
            String line = bufferedReader.readLine();
            String[] list = line.split("\\s+");
            return new Value(list);
        }catch(IOException e)
        {
            e.printStackTrace();
            return new Value();
        }
    }

//    private static Value numericOperate(WordStream stream, IntBinaryOperator intBinaryOperator, DoubleBinaryOperator doubleBinaryOperator)
//            throws RunningException, SyntaxException
//    {
//        Value first = Interpreter.value(stream);
//        Value second = Interpreter.value(stream);
//        try
//        {
//            int a = first.toInt();
//            int b = second.toInt();
//            return new Value(String.valueOf(intBinaryOperator.applyAsInt(a, b)));
//        } catch (NumberFormatException e)
//        {
//            try
//            {
//                double a = first.toFloat();
//                double b = second.toFloat();
//                return new Value(String.valueOf(doubleBinaryOperator.applyAsDouble(a, b)));
//            } catch (NumberFormatException f)
//            {
//                throw new RunningException("One of the operand cannot convert to numeric.");
//            }
//        }
//    }
//
//    private static Value compare(WordStream stream, IntBinaryComparator intBinaryComparator, DoubleBinaryComparator doubleBinaryComparator, StringComparator stringBinaryComparator)
//            throws RunningException, SyntaxException
//    {
//        Value first = Interpreter.value(stream);
//        Value second = Interpreter.value(stream);
//        try
//        {
//            int a = first.toInt();
//            int b = second.toInt();
//            return new Value(String.valueOf(intBinaryComparator.apply(a, b)));
//        } catch (NumberFormatException e)
//        {
//            try
//            {
//                double a = first.toFloat();
//                double b = second.toFloat();
//                return new Value(String.valueOf(doubleBinaryComparator.apply(a, b)));
//            } catch (NumberFormatException f)
//            {
//                return new Value(String.valueOf(stringBinaryComparator.apply(first.toString(), second.toString())));
//            }
//        }
//    }
    private static Value numericOperate(WordStream stream, NumericOperator numericOperator)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream);
        Value second = Interpreter.value(stream);
        try
        {
            BigDecimal a = first.toNumeric();
            BigDecimal b = second.toNumeric();
            return new Value(numericOperator.apply(a,b).toString());
        }
        catch (NumberFormatException e)
        {
            throw new RunningException("One of the operand cannot convert to numeric.");
        }
    }

    private static Value compare(WordStream stream, NumericComparator numericComparator, StringComparator stringComparator)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream);
        Value second = Interpreter.value(stream);
        try
        {
            BigDecimal a = first.toNumeric();
            BigDecimal b = second.toNumeric();
            return new Value(numericComparator.apply(a,b).toString());
        }
        catch (NumberFormatException e)
        {
            String a = first.toString();
            String b = first.toString();
            return new Value(stringComparator.apply(a,b).toString());
        }
    }

    private static Value booleanOperate(WordStream stream, BooleanBinaryOperator booleanBinaryOperator)
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
        //return numericOperate(stream, (int a, int b) -> a + b, (double a, double b) -> a + b);
        return numericOperate(stream, BigDecimal::add);
    }

    static Value sub(WordStream stream)
            throws RunningException, SyntaxException
    {
        //return numericOperate(stream, (int a, int b) -> a - b, (double a, double b) -> a - b);
        return numericOperate(stream, BigDecimal::subtract);
    }

    static Value mul(WordStream stream)
            throws RunningException, SyntaxException
    {
        //return numericOperate(stream, (int a, int b) -> a * b, (double a, double b) -> a * b);
        return numericOperate(stream, BigDecimal::multiply);
    }

    static Value div(WordStream stream)
            throws RunningException, SyntaxException
    {
        try
        {
            //return numericOperate(stream, (int a, int b) -> a / b, (double a, double b) -> a / b);
            return numericOperate(stream, (BigDecimal a, BigDecimal b) -> a.divide(b, 16, BigDecimal.ROUND_HALF_EVEN));
        }
        catch(ArithmeticException e)
        {
            throw new RunningException("Cannot divided by zero");
        }

    }

    static Value mod(WordStream stream)
            throws RunningException, SyntaxException
    {
        //return numericOperate(stream, (int a, int b) -> a % b, (double a, double b) -> a % b);
        try{
            return numericOperate(stream, BigDecimal::remainder);
        }
        catch(ArithmeticException e)
        {
            throw new RunningException("Cannot divided by zero");
        }

    }

    static Value eq(WordStream stream)
            throws RunningException, SyntaxException
    {
        //return compare(stream, Integer::equals, Double::equals, String::equals);
        return compare(stream, BigDecimal::equals, String::equals);
    }

    static Value lt(WordStream stream)
            throws RunningException, SyntaxException
    {
        //return compare(stream, (Integer a, Integer b) -> a.compareTo(b) < 0, (Double a, Double b) -> a.compareTo(b) < 0, (String a, String b) -> a.compareTo(b) < 0);
        return compare(stream, (BigDecimal a, BigDecimal b) -> a.compareTo(b) < 0, (String a, String b) -> a.compareTo(b) < 0);
    }

    static Value gt(WordStream stream)
            throws RunningException, SyntaxException
    {
        //return compare(stream, (Integer a, Integer b) -> a.compareTo(b) > 0, (Double a, Double b) -> a.compareTo(b) > 0, (String a, String b) -> a.compareTo(b) > 0);
        return compare(stream, (BigDecimal a, BigDecimal b) -> a.compareTo(b) > 0, (String a, String b) -> a.compareTo(b) > 0);
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
