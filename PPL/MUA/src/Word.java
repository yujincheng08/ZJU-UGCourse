import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.EmptyStackException;
import java.util.Iterator;
import java.util.Stack;

class Word extends Value {
    private String value;

    Word() {
        this.value = "";
    }

    Word(String value) {
        this.value = value;
    }

    private void popExpressionStack(Stack<Value> valueStack, Stack<String> opStack, int level)
            throws RunningException, SyntaxException {
        while (true) {
            try {
                if (opStack.empty())
                    break;
                String op = opStack.pop();
                Value second = valueStack.pop();
                Value first;
                switch (op) {
                    case "+":
                        first = valueStack.pop();
                        valueStack.push(Function.numericOperate(first, second, BigDecimal::add));
                        break;
                    case "-":
                        first = valueStack.pop();
                        valueStack.push(Function.numericOperate(first, second, BigDecimal::subtract));
                        break;
                    case "*":
                        first = valueStack.pop();
                        valueStack.push(Function.numericOperate(first, second, BigDecimal::multiply));
                        break;
                    case "/":
                        first = valueStack.pop();
                        valueStack.push(Function.numericOperate(first, second, (BigDecimal a, BigDecimal b) -> a.divide(b, 16, BigDecimal.ROUND_HALF_EVEN)));
                        break;
                    case "%":
                        first = valueStack.pop();
                        valueStack.push(Function.numericOperate(first, second, BigDecimal::remainder));
                        break;
                    case "(":
                        if (level == 0) {
                            valueStack.push(second);
                            level = 1;
                        } else
                            throw new SyntaxException("Illegal Expression");
                }
                if (level == 1 && !opStack.empty() && opStack.peek().matches("([+\\-(])"))
                    break;
            } catch (EmptyStackException e) {
                throw new SyntaxException("Illegal Expression");
            }
        }
    }

    Word(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException {
        Stack<Value> valueStack = new Stack<>();
        Stack<String> opStack = new Stack<>();
        int valueSign = 0; // Default 0. 0 and 1 for positive, -1 for negative.
        try {
            while (true) {
                String first = stream.next();
                for (String c : first.split("((?<=\\+)|(?=\\+))|((?<=-)|(?=-))|((?<=\\*)|(?=\\*))|((?<=/)|(?=/))|((?<=%)|(?=%))")) {
                    if (c.length() == 0)
                        continue;
                    switch (c) {
                        case "(":
                        case "*":
                        case "/":
                        case "%":
                            opStack.push(c);
                            valueSign = 1;
                            break;
                        case "-":
                            if (valueSign != 0) {
                                valueSign = -valueSign;
                            }
                        case "+":
                            if (valueSign == 0) {
                                valueSign = 1;
                                String peek = opStack.peek();
                                if (peek.equals("*") || peek.equals("/") || peek.equals("%"))
                                    popExpressionStack(valueStack, opStack, 1);
                                opStack.push(c);
                            }
                            break;
                        case ")":
                            popExpressionStack(valueStack, opStack, 0);
                            break;
                        default:
                            Value result = Interpreter.value(stream.putBack(c), wordList);
                            if (valueSign < 0)
                                result = Function.numericOperate(ZERO, result, BigDecimal::subtract);
                            valueStack.push(result);
                            valueSign = 0;
                            String peek = opStack.peek();
                            if (peek.equals("*") || peek.equals("/") || peek.equals("%"))
                                popExpressionStack(valueStack, opStack, 1);
                            break;
                    }
                }
                if (valueStack.size() == 1 && opStack.size() == 0) {
                    value = valueStack.pop().toString();
                    break;
                }
            }
        } catch (EmptyStackException e) {
            throw new SyntaxException("Illegal Expression.");
        }
    }

    Word(Boolean bool) {
        this.value = bool.toString();
    }

    Word(BigDecimal bigDecimal) {
        this.value = bigDecimal.toString();
    }

    @Override
    WordStream toWordStream() {
        return new WordStream(value, false);
    }

    @Override
    boolean isList() {
        return false;
    }

    @Override
    boolean isBool() {
        switch (value) {
            case "true":
            case "false":
                return true;
            default:
                return false;
        }
    }

    @Override
    boolean isEmpty() {
        return value.isEmpty();
    }

    @Override
    boolean isNumber() {
        try {
            new BigDecimal(value);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    @Override
    int size() {
        return value.length();
    }

    @Override
    BigDecimal toNumeric()
            throws RunningException {
        try {
            return new BigDecimal(value);
        } catch (NumberFormatException e) {
            throw new RunningException(value + " cannot be converted to numeric");
        }
    }

    @Override
    boolean toBoolean() throws RunningException {
        switch (value) {
            case "true":
                return true;
            case "false":
                return false;
            default:
                throw new RunningException(value + " cannot be converted to boolean");
        }
    }

    @Override
    Value item(BigInteger i)
            throws RunningException {
        if (value.length() > 0 && i.compareTo(BigInteger.valueOf(value.length())) < 0)
            return item(i.intValue());
        else
            throw new RunningException(i + " out of bound.");
    }

    @Override
    Value item(int i) {
        if (i < value.length())
            return new Word(String.valueOf(value.charAt(i)));
        else
            return null;
    }

    @Override
    Value but(int i) {
        StringBuilder stringBuilder = new StringBuilder();
        for (int j = 0; j < value.length(); ++j)
            if (i != j)
                stringBuilder.append(value.charAt(j));
        return new Word(stringBuilder.toString());
    }

    @Override
    BigInteger toInt() throws RunningException {
        try {
            return new BigInteger(value);
        } catch (NumberFormatException e) {
            throw new RunningException(value + " cannot be converted to integer.");
        }
    }

    @Override
    public String toString() {
        return value;
    }

    @Override
    public Iterator<Value> iterator() {
        ArrayList<Value> tmp = new ArrayList<>();
        tmp.add(this);
        return tmp.iterator();
    }
}
