import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.EmptyStackException;
import java.util.Stack;

public class Value extends ArrayList<String>
{
    static final Value ZERO = new Value("0");
    WordStream toWordStream()
    {
        return new WordStream(toArray(new String[0]), false);
    }

    Value()
    {
        super();
    }

    Value(String value)
    {
        this();
        this.add(value);
    }

    void add(StringBuilder stringBuilder) {
        if(stringBuilder.length() > 0)
            add(stringBuilder.toString());
        stringBuilder.setLength(0);
    }

    private void buildList(String first, WordStream stream)
    {
        int count = 0;
        while(true)
        {
            switch (first) {
                case "[":
                    if (++count > 1)
                        add(first);
                    break;
                case "]":
                    if (count-- > 1)
                        add(first);
                    break;
                default:
                    add(first);
                    break;
            }
            if(count>0)
                first = stream.next();
            else
                break;
        }
    }

    private void popExpressionStack(Stack<Value> valueStack, Stack<String> opStack, int level)
            throws RunningException, SyntaxException
    {
        while(true)
        {
            try{
                if(opStack.empty())
                    break;
                String op = opStack.pop();
                Value second = valueStack.pop();
                Value first;
                switch(op)
                {
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
                        if(level==0) {
                            valueStack.push(second);
                            level = 1;
                        }
                        else
                            throw new SyntaxException("Illegal Expression");
                }
                if(level == 1 && !opStack.empty() && opStack.peek().matches("(\\+|-|\\()"))
                    break;
            }catch(EmptyStackException e)
            {
                throw new SyntaxException("Illegal Expression");
            }
        }
    }

    private void buildExpression(String first, WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        Stack<Value> valueStack = new Stack<>();
        Stack<String> opStack = new Stack<>();
        int valueSign = 0; // Default 0. 0 and 1 for positive, -1 for negative.
        try {
            while (true) {
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
                    add(valueStack.pop().toString());
                    break;
                } else
                    first = stream.next();
            }
        }
        catch (EmptyStackException e)
        {
            throw new SyntaxException("Illegal Expression.");
        }
    }

    Value(String first, WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        switch (first) {
            case "[":
                buildList(first, stream);
                break;
            case "(":
                buildExpression(first, stream, wordList);
                break;
            default:
                add(first);
                break;
        }
    }

    Value(String[] values)
    {
        super(Arrays.asList(values));
    }

    public Value clone()
    {
        return (Value) super.clone();
    }

    private void checkSizeForValue(String targetType)
            throws RunningException
    {
        if(size() == 0)
            throw new RunningException("Cannot convert null to "+targetType+".");
        else if(size() > 1)
            throw new RunningException("Cannot convert list to "+targetType+".");
    }

    Boolean toBoolean()
            throws RunningException
    {
        checkSizeForValue("boolean");
        String value = get(0);
        switch(value){
            case "true":
                return true;
            case "false":
                return false;
            default:
                throw new RunningException("Cannot convert " + value + " to boolean");
        }
    }

//    public int toInt()
//            throws RunningException, NumberFormatException
//    {
//        checkSizeForValue(1, "integer");
//        String value = get(0);
//        return Integer.parseInt(value);
//    }
//
//    public double toFloat()
//            throws RunningException, NumberFormatException
//    {
//        checkSizeForValue(1, "float");
//        String value = get(0);
//        return Double.parseDouble(value);
//    }

    BigDecimal toNumeric()
            throws RunningException, NumberFormatException
    {
        checkSizeForValue("numeric");
        String value = get(0);
        return new BigDecimal(value);
    }

    @Override
    public String toString()
    {
        StringBuilder result = new StringBuilder();
        for (String value: this)
        {
            result.append(value).append(" ");
        }
        return size()>1 ? "[ " + result.toString().trim() + " ]" : result.toString().trim();
    }
}
