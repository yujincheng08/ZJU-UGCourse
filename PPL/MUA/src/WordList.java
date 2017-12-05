import java.util.HashMap;

class WordList
{
    private HashMap<String, Word> list;
    WordList()
    {
        list = new HashMap<>();
    }

    Value thing(String name) throws SyntaxException, RunningException
    {
        Word word = list.get(name);
        if(word == null)
            throw new RunningException("Word " +name+ " does not exist.");
        else
            return word.getValue();
    }

    void make(String name, Value value)
    {
        list.put(name, new Word(value));
    }

    void make(String name, WordStream stream)
            throws RunningException, SyntaxException
    {
        Word word = list.get(name);
        //word = new Word();
        Word newWord = new Word(Interpreter.value(stream, this));
        if(word != null)
        {
            list.remove(name);
        }
        list.put(name, newWord);
    }

    Value isname(String name)
    {
        if(list.containsKey(name))
            return new Value("true");
        else
            return new Value("false");
    }

    void erase(String name)
        throws RunningException
    {
        if(list.containsKey(name))
            list.remove(name);
        else
            throw new RunningException("Word " + name + "does not exist.");
    }

    boolean contains(String name)
    {
        return list.containsKey(name);
    }

}
