import java.util.ArrayList;
public class Word
{
    Value value;
    Word(WordStream stream)
            throws RunningException, SyntaxException
    {
        value = new Value();
        setValue(stream);
    }

    Word(Value value)
    {
        this.value = value;
    }

    public void setValue(WordStream stream)
            throws RunningException, SyntaxException
    {
        //String word = stream.next();
        String word = Interpreter.value(stream).toString();
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
            value.add(word);
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
