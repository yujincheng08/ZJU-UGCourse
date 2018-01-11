import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.Map;

import static java.lang.Thread.sleep;

class Function {
    private static String getWordName(WordStream stream, WordList wordList)
            throws SyntaxException, RunningException {
        Value value = Interpreter.value(stream, wordList);
        String name = value.toString();
        if (value.isList() || !name.matches("[_a-zA-Z].*"))
            throw new SyntaxException("Except a word literary but get " + name);
        else
            return name;
    }

    static Value thing(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        return thing(getWordName(stream, wordList), wordList);
    }

    static Value thing(String name, WordList wordList)
            throws RunningException {
        return wordList.thing(name);
    }

    static Value isName(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        return wordList.isname(getWordName(stream, wordList));
    }

    static Value read(WordStream stream) {
        System.out.print("Input a word: ");
        WordStream tmp = new WordStream();
        String word = tmp.next();
        stream.merge(tmp);
        return new Word(word);
    }

    static Value readLinst() {
        System.out.print("Input a list: > ");
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));
        try {
            String line = bufferedReader.readLine();
            String[] list = line.split("\\s+");
            return new List(list);
        } catch (IOException e) {
            e.printStackTrace();
            return new Word();
        }
    }

    static Value numericOperate(Value first, Value second, NumericOperator numericOperator)
            throws RunningException {
        BigDecimal a = first.toNumeric();
        BigDecimal b = second.toNumeric();
        return new Word(numericOperator.apply(a, b));
    }

    private static Value numericOperate(WordStream stream, NumericOperator numericOperator, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        return numericOperate(first, second, numericOperator);
    }

    private static Value compare(WordStream stream, NumericComparator numericComparator, StringComparator stringComparator, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        if (first.isNumber() && second.isNumber()) {
            BigDecimal a = first.toNumeric();
            BigDecimal b = second.toNumeric();
            return new Word(numericComparator.apply(a, b).toString());
        } else {
            String a = first.toString();
            String b = second.toString();
            return new Word(stringComparator.apply(a, b));
        }
    }

    private static Value booleanOperate(WordStream stream, BooleanBinaryOperator booleanBinaryOperator, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        Boolean a = first.toBoolean();
        Boolean b = second.toBoolean();
        return new Word(String.valueOf(booleanBinaryOperator.apply(a, b)));
    }

    static Value add(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        //return numericOperate(stream, (int a, int b) -> a + b, (double a, double b) -> a + b);
        return numericOperate(stream, BigDecimal::add, wordList);
    }

    static Value sub(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        //return numericOperate(stream, (int a, int b) -> a - b, (double a, double b) -> a - b);
        return numericOperate(stream, BigDecimal::subtract, wordList);
    }

    static Value mul(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        //return numericOperate(stream, (int a, int b) -> a * b, (double a, double b) -> a * b);
        return numericOperate(stream, BigDecimal::multiply, wordList);
    }

    static Value div(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        try {
            //return numericOperate(stream, (int a, int b) -> a / b, (double a, double b) -> a / b);
            return numericOperate(stream, (BigDecimal a, BigDecimal b) -> a.divide(b, 16, BigDecimal.ROUND_HALF_EVEN), wordList);
        } catch (ArithmeticException e) {
            throw new RunningException("Cannot divided by zero");
        }

    }

    static Value mod(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        //return numericOperate(stream, (int a, int b) -> a % b, (double a, double b) -> a % b);
        try {
            return numericOperate(stream, BigDecimal::remainder, wordList);
        } catch (ArithmeticException e) {
            throw new RunningException("Cannot divided by zero");
        }

    }

    static Value eq(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        //return compare(stream, Integer::equals, Double::equals, String::equals);
        return compare(stream, BigDecimal::equals, String::equals, wordList);
    }

    static Value lt(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        //return compare(stream, (Integer a, Integer b) -> a.compareTo(b) < 0, (Double a, Double b) -> a.compareTo(b) < 0, (String a, String b) -> a.compareTo(b) < 0);
        return compare(stream, (BigDecimal a, BigDecimal b) -> a.compareTo(b) < 0, (String a, String b) -> a.compareTo(b) < 0, wordList);
    }

    static Value gt(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        //return compare(stream, (Integer a, Integer b) -> a.compareTo(b) > 0, (Double a, Double b) -> a.compareTo(b) > 0, (String a, String b) -> a.compareTo(b) > 0);
        return compare(stream, (BigDecimal a, BigDecimal b) -> a.compareTo(b) > 0, (String a, String b) -> a.compareTo(b) > 0, wordList);
    }

    static Value and(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        return booleanOperate(stream, Boolean::logicalAnd, wordList);
    }

    static Value or(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        return booleanOperate(stream, Boolean::logicalOr, wordList);
    }

    static Value not(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Boolean a = first.toBoolean();
        return new Word(String.valueOf(!a));
    }

    static Value random(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        try {
            BigDecimal max = first.toNumeric();
            if (max.compareTo(new BigDecimal("0")) <= 0)
                throw new RunningException("Range errors.");
            BigDecimal randFromDouble = new BigDecimal(Math.random());
            BigDecimal actualRandomDec = randFromDouble.multiply(max);
            return new Word(actualRandomDec.toString());
        } catch (NumberFormatException e) {
            throw new RunningException(first.toString() + " cannot be converted to numeric");
        }
    }

    private static BigDecimal bigDecimalSqrt(BigDecimal n, BigDecimal ni, BigDecimal precision) {
        BigDecimal f = ni.pow(2).subtract(n);
        BigDecimal df = ni.multiply(new BigDecimal(2));
        BigDecimal f_df = f.divide(df, 300, BigDecimal.ROUND_HALF_DOWN);
        ni = ni.subtract(f_df);
        BigDecimal currentSquare = ni.pow(2);
        BigDecimal currentPrecision = currentSquare.subtract(n);
        currentPrecision = currentPrecision.abs();
        if (currentPrecision.compareTo(precision) <= -1) {
            return ni;
        }
        return bigDecimalSqrt(n, ni, precision);
    }

    private static BigDecimal bigDecimalSqrt(BigDecimal c) {
        //noinspection BigDecimalMethodWithoutRoundingCalled
        return bigDecimalSqrt(c, BigDecimal.ONE, BigDecimal.ONE.divide(BigDecimal.TEN.pow(300))).setScale(16, BigDecimal.ROUND_HALF_EVEN);
    }

    static Value sqrt(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        BigDecimal a = first.toNumeric();
        if (a.compareTo(BigDecimal.ZERO) < 0)
            throw new RunningException(first.toString() + " is less than zero.");
        return new Word(bigDecimalSqrt(a));

    }

    static Value isnumber(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        if (first.isNumber())
            return Value.TRUE;
        else
            return Value.FALSE;
    }

    static Value islist(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        if (first.isList())
            return Value.TRUE;
        else
            return Value.FALSE;
    }

    static Value isbool(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        if (first.isBool())
            return Value.TRUE;
        else
            return Value.FALSE;
    }

    static Value isempty(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        if (first.isEmpty())
            return Value.TRUE;
        else
            return Value.FALSE;
    }


    static Value word(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        if(first.isList() || second.isList())
            throw new RunningException("Word can only concatenate two words.");
        return new Word(first.toString() + second.toString());
    }

    static Value list(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        return List.list(first, second);
    }

    static Value sentence(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        return List.sentence(first, second);
    }

    static Value join(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        return List.join(first, second);
    }

    static Value item(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        Value second = Interpreter.value(stream, wordList);
        BigInteger index = first.toInt();
        return second.item(index);
    }


    static Value first(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        return first.item(BigInteger.ZERO);
    }

    static Value last(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        return first.item(BigInteger.valueOf(first.size() - 1));
    }

    static Value butfirst(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        return first.but(0);
    }

    static Value butlast(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        return first.but(first.size() - 1);
    }

    static Value Int(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value first = Interpreter.value(stream, wordList);
        BigDecimal value = first.toNumeric();
        return new Word(value.setScale(0, BigDecimal.ROUND_FLOOR));
    }

    static void make(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        wordList.make(getWordName(stream, wordList), stream);
    }

    static void erase(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        wordList.erase(getWordName(stream, wordList));
    }

    static void print(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        System.out.println(Interpreter.value(stream, wordList).toString());
    }

    static void output(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        wordList.make(WordList.outputWordName, stream);
    }

    static Value run(String functionName, WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        if (wordList.contains(functionName)) {
            Value function = wordList.thing(functionName);
            if (!function.isList() || function.isEmpty())
                throw new RunningException(functionName + " is not a function");
            wordList.make(functionName, function);
            Value functionArg = function.item(0);
            if (functionArg == null || !functionArg.isList())
                throw new RunningException(functionName + " is not a function");
            Value functionBody = function.item(1);
            if (functionBody == null || !functionBody.isList())
                throw new RunningException(functionName + " is not a function");
            Value dummy = function.item(2);
            if (dummy != null)
                throw new RunningException(functionName + " is not a function");
            try {
                wordList.newSpace();
                for (Value arg : functionArg) {
                    if (arg.isList()) {
                        throw new RunningException("List is not a valid argument.");
                    }
                    Value v = Interpreter.value(stream, wordList);
                    wordList.make(arg.toString(), v);
                }
                Interpreter.interpret(functionBody.toWordStream(), wordList);
                if (wordList.contains(WordList.outputWordName)) {
                    return wordList.getOutput();
                } else {
                    return null;
                }
            }catch(RunningException | SyntaxException e){
                throw e;
            }finally {
                wordList.endSpace();
            }

        } else
            throw new RunningException("Unexpected token: " + functionName);
    }

    static void run(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        Value runBody = Interpreter.value(stream, wordList);
        if(!runBody.isList())
            throw new RunningException("Only list can be run.");
        stream.putBack(runBody.toWordStream());
    }

    static void repeat(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value timesValue = Interpreter.value(stream, wordList);
        BigInteger times = timesValue.toInt();
        Value loopBody = Interpreter.value(stream, wordList);
        if(!loopBody.isList())
            throw new RunningException("Only list can be repeated.");
        WordStream loopBodyStream = loopBody.toWordStream();
        for (BigInteger i = BigInteger.ZERO; i.compareTo(times) < 0; i = i.add(BigInteger.ONE))
            stream.putBack(loopBodyStream);
    }

    static void export(WordList wordList)
    {
        for(Map.Entry<String, Value> entry : wordList)
            if(!entry.getKey().equals(WordList.outputWordName))
                Interpreter.globalWordList.make(entry.getKey(), entry.getValue());
    }

    static void wait(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value timeValue = Interpreter.value(stream, wordList);
        long i = timeValue.toInt().longValue();
        try {
            sleep(i);
        } catch (InterruptedException e) {
            throw new RunningException("Sleeping was interrupted.");
        }
    }

    static void save(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value path = Interpreter.value(stream, wordList);
        if (path.isList())
            throw new RunningException("List is not a valid file name");
        wordList.save(path.toString());
    }

    static void load(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Value path = Interpreter.value(stream, wordList);
        if (path.isList())
            throw new RunningException("List is not a valid file name");
        wordList.load(path.toString());
    }

    static void erall(WordList wordList) {
        wordList.clear();

    }

    static void poall(WordList wordList) {
        wordList.print();
    }

    static void If(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        Value value = Interpreter.value(stream, wordList);
        if (!value.isBool())
            throw new RunningException("Expected boolean value, but got " + value.toString());
        Value trueSection = Interpreter.value(stream, wordList);
        Value falseSection = Interpreter.value(stream, wordList);
        if(!trueSection.isList())
            throw new RunningException("Expected a list, but got " + trueSection.toString());
        if(!falseSection.isList())
            throw new RunningException("Expected a list, but got " + falseSection.toString());
        switch (value.toString())
        {
            case "true":
                stream.putBack(trueSection.toWordStream());
                break;
            case "false":
                stream.putBack(falseSection.toWordStream());
                break;
        }
    }

}
