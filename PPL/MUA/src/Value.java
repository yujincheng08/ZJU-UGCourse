import java.util.ArrayList;
import java.util.Arrays;

public class Value extends ArrayList<String>
{
    public WordStream toWordStream()
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

    Value(String[] values)
    {
        super(Arrays.asList(values));
    }

    public Value clone()
    {
        Value value = (Value) super.clone();
        return value;
    }

    private void checkSizeForValue(int size, String targetType)
            throws RunningException
    {
        if(size() == 0)
            throw new RunningException("Cannot convert null to "+targetType+".");
        else if(size() > size)
            throw new RunningException("Cannot convert list to "+targetType+".");
    }

    public Boolean toBoolean()
            throws RunningException
    {
        checkSizeForValue(1, "boolean");
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

    public int toInt()
            throws RunningException, NumberFormatException
    {
        checkSizeForValue(1, "integer");
        String value = get(0);
        return Integer.parseInt(value);
    }

    public double toFloat()
            throws RunningException, NumberFormatException
    {
        checkSizeForValue(1, "float");
        String value = get(0);
        return Double.parseDouble(value);
    }
}
