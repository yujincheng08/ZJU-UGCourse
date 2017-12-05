import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigDecimal;
import java.math.BigInteger;

class Function
{
    static final String outputWordName = "0";
    private static String getWordName(WordStream stream, WordList wordList)
            throws SyntaxException, RunningException
    {
        Value value = Interpreter.value(stream, wordList);
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

    static Value thing(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        return thing(getWordName(stream, wordList), wordList);
    }

    static Value thing(String name, WordList wordList)
            throws RunningException, SyntaxException
    {
        return wordList.thing(name);
    }

    static Value isName(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        return wordList.isname(getWordName(stream, wordList));
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

    static Value numericOperate(Value first, Value second, NumericOperator numericOperator)
            throws RunningException, SyntaxException
    {
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

    private static Value numericOperate(WordStream stream, NumericOperator numericOperator, WordList wordList)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        return numericOperate(first, second, numericOperator);
    }

    private static Value compare(WordStream stream, NumericComparator numericComparator, StringComparator stringComparator, WordList wordList)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
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

    private static Value booleanOperate(WordStream stream, BooleanBinaryOperator booleanBinaryOperator, WordList wordList)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        Boolean a = first.toBoolean();
        Boolean b = second.toBoolean();
        return new Value(String.valueOf(booleanBinaryOperator.apply(a, b)));
    }

    static Value add(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        //return numericOperate(stream, (int a, int b) -> a + b, (double a, double b) -> a + b);
        return numericOperate(stream, BigDecimal::add, wordList);
    }

    static Value sub(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        //return numericOperate(stream, (int a, int b) -> a - b, (double a, double b) -> a - b);
        return numericOperate(stream, BigDecimal::subtract, wordList);
    }

    static Value mul(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        //return numericOperate(stream, (int a, int b) -> a * b, (double a, double b) -> a * b);
        return numericOperate(stream, BigDecimal::multiply, wordList);
    }

    static Value div(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        try
        {
            //return numericOperate(stream, (int a, int b) -> a / b, (double a, double b) -> a / b);
            return numericOperate(stream, (BigDecimal a, BigDecimal b) -> a.divide(b, 16, BigDecimal.ROUND_HALF_EVEN), wordList);
        }
        catch(ArithmeticException e)
        {
            throw new RunningException("Cannot divided by zero");
        }

    }

    static Value mod(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        //return numericOperate(stream, (int a, int b) -> a % b, (double a, double b) -> a % b);
        try{
            return numericOperate(stream, BigDecimal::remainder, wordList);
        }
        catch(ArithmeticException e)
        {
            throw new RunningException("Cannot divided by zero");
        }

    }

    static Value eq(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        //return compare(stream, Integer::equals, Double::equals, String::equals);
        return compare(stream, BigDecimal::equals, String::equals, wordList);
    }

    static Value lt(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        //return compare(stream, (Integer a, Integer b) -> a.compareTo(b) < 0, (Double a, Double b) -> a.compareTo(b) < 0, (String a, String b) -> a.compareTo(b) < 0);
        return compare(stream, (BigDecimal a, BigDecimal b) -> a.compareTo(b) < 0, (String a, String b) -> a.compareTo(b) < 0, wordList);
    }

    static Value gt(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        //return compare(stream, (Integer a, Integer b) -> a.compareTo(b) > 0, (Double a, Double b) -> a.compareTo(b) > 0, (String a, String b) -> a.compareTo(b) > 0);
        return compare(stream, (BigDecimal a, BigDecimal b) -> a.compareTo(b) > 0, (String a, String b) -> a.compareTo(b) > 0, wordList);
    }

    static Value and(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        return booleanOperate(stream, Boolean::logicalAnd, wordList);
    }

    static Value or(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        return booleanOperate(stream, Boolean::logicalOr, wordList);
    }

    static Value not(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        Value first = Interpreter.value(stream, wordList);
        Boolean a = first.toBoolean();
        return new Value(String.valueOf(!a));
    }

    static void make(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        wordList.make(getWordName(stream, wordList), stream);
    }

    static void erase(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        wordList.erase(getWordName(stream, wordList));
    }

    static void print(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        System.out.println(Interpreter.value(stream, wordList).toString());
    }

    static void output(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        wordList.make(outputWordName, stream);
    }

    static Value run(String functionName, WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        if(wordList.contains(functionName))
        {
            WordList localWordList = new WordList();
            Value function = wordList.thing(functionName);
            WordStream functionStream = function.toWordStream();
            localWordList.make(functionName, function);
            String first = functionStream.next();
            if(first == null || !first.startsWith("["))
                throw new RunningException(functionName + " is not a function");
            Value functionArg = new Value(first, functionStream, null);
            first = functionStream.next();
            if(first == null || !first.startsWith("["))
                throw new RunningException(functionName + " is not a function");
            Value functionBody = new Value(first, functionStream, null);
            first = functionStream.next();
            if(first != null)
                throw new RunningException(functionName + " is not a function");
            for(String arg : functionArg)
            {
                localWordList.make(arg, stream);
            }

            //Interpreter.value(functionBody.toWordStream(), localWordList);

            Interpreter.interpret(functionBody.toWordStream(), localWordList);// TODO
            if(localWordList.contains(outputWordName))
                return localWordList.thing(outputWordName);
            else
                return null;
        }
        else
            throw new RunningException("Unexpected token: " + functionName);
    }

    static void repeat(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        Value timesValue = Interpreter.value(stream, wordList);
        int times;
        try {
            times = Integer.parseInt(timesValue.toString());
        } catch (NumberFormatException e)
        {
            throw new RunningException("Repeat times " + timesValue.toString()+ " is not an integer");
        }
        Value loopBody = Interpreter.value(stream, wordList);
        for(int i = 0; i < times; ++i)
            Interpreter.interpret(loopBody.toWordStream(), wordList);

    }

    static Value run(WordStream stream)
    {
        return null;
    }
}
