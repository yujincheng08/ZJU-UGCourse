import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Arrays;

public class Value extends ArrayList<String>
{
    WordStream toWordStream()
    {
        return new WordStream(toArray(new String[0]));
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

    Value(String first, WordStream stream)
    {
        int count = 0;
        StringBuilder value = new StringBuilder();
        while(true)
        {
            for (int i = 0; i < first.length(); ++i)
            {
                char c = first.charAt(i);
                if (c == '[')
                {
                    add(value);
                    if(++count > 1)
                        add("[");
                }
                else if (c == ']')
                {
                    add(value);
                    if(count-- > 1)
                        add("]");
                }
                else if(c == ' ')
                {
                    add(value);
                }
                else
                {
                    value.append(c);
                }
            }
            if(count>0)
                first = stream.next();
            else
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
