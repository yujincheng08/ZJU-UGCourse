import java.util.HashMap;
public class WordList
{
    private HashMap<String, Word> list;
    WordList()
    {
        list = new HashMap<>();
    }

    Value thing(String name) throws SyntaxException, RunningException
    {
        if(name.matches("-?\\d+"))
            throw new SyntaxException("Cannot thing a number.");
        Word word = list.get(name);
        if(word == null)
            throw new RunningException("Word " +name+ " not exist.");
        else
            return word.getValue();
    }

    void make(String name, WordStream stream)
    {
        Word word = list.get(name);
        if(word == null)
        {
            word = new Word(stream);
            list.put(name, word);
        }
        else
        {
            word.setValue(stream);
        }
    }

    Value isname(String name)
    {
        if(list.containsKey(name))
            return new Value("true");
        else
            return new Value("false");
    }
}
