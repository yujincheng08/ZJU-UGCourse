import java.util.ArrayList;
public class Word
{
    private boolean isNum;
    Value value;
    Word(WordStream stream)
    {
        value = new Value();
        setValue(stream);
    }

    Word(String value)
    {
        this.value = new Value();
        isNum = false;
        this.value.add(value);
    }

    public void setValue(WordStream stream)
    {
        isNum = false;
        String word = stream.next();
        if(word.startsWith("["))
        {
            word = word.substring(1);
            if(word.length() == 0)
                word = stream.next();
            while(!word.endsWith("]"))
            {
                value.add(word);
                word = stream.next();
            }
            if(word.length() > 1)
                value.add(word.substring(0,word.length()-1));
        }
        else
        {
            value.add(word);
            if(word.matches("-?\\d+(\\.\\d+)?"))
                isNum = true;
        }
    }

    public void setValue(Value value)
    {
        this.value = value.clone();
    }

    public Value getValue()
    {
        return value;
    }
}
