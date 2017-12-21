import java.io.*;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

class WordList implements Iterable<Map.Entry<String, Value>>{
    static final String outputWordName = "0";
    // static final String testWordName = "1";
    private HashMap<String, Value> list;

    WordList() {
        list = new HashMap<>();
    }

    Value thing(String name) throws RunningException {
        Value value = list.get(name);
        if (value == null)
            throw new RunningException("Word " + name + " does not exist.");
        else
            return value;
    }

    void make(String name, Value value) {
        list.put(name, value);
    }

    void make(String name, WordStream stream)
            throws RunningException, SyntaxException {
        Value value = list.get(name);
        //word = new Word();
        Value newValue = Interpreter.value(stream, this);
        if (value != null) {
            list.remove(name);
        }
        list.put(name, newValue);
    }

    Value isname(String name) {
        if (list.containsKey(name))
            return Value.TRUE;
        else
            return Value.FALSE;
    }

    void erase(String name)
            throws RunningException {
        if (list.containsKey(name))
            list.remove(name);
        else
            throw new RunningException("Word " + name + "does not exist.");
    }

    void save(String path)
            throws RunningException {
        Value output = null;
//        Value test = null;
        if (list.containsKey(outputWordName)) {
            output = list.get(outputWordName);
            list.remove(outputWordName);
        }
//        if (list.containsKey(testWordName)) {
//            test = list.get(testWordName);
//            list.remove(testWordName);
//        }
        File file = new File(path);
        try {
            if (!file.exists())
                //noinspection ResultOfMethodCallIgnored
                file.createNewFile();
            FileOutputStream fileOutputStream = new FileOutputStream(file);
            ObjectOutputStream objectOutputStream = new ObjectOutputStream(fileOutputStream);
            objectOutputStream.writeObject(list);
        } catch (FileNotFoundException e) {
            throw new RunningException("File " + path + " not exists.");
        } catch (IOException e) {
            throw new RunningException(e.toString());
        } finally {
            if (output != null)
                list.put(outputWordName, output);
//            if (test != null)
//                list.put(testWordName, test);
        }
    }

    void load(String path)
            throws RunningException {
        Value output = null;
//        Value test = null;
//        if (list.containsKey(outputWordName))
//            output = list.get(outputWordName);
//        if (list.containsKey(testWordName))
//            test = list.get(testWordName);
        File file = new File(path);
        HashMap<String, Value> newList = null;
        try {
            FileInputStream fileInputStream = new FileInputStream(file);
            ObjectInputStream objectInputStream = new ObjectInputStream(fileInputStream);
            //noinspection unchecked
            newList = (HashMap<String, Value>) objectInputStream.readObject();
        } catch (FileNotFoundException e) {
            throw new RunningException("File " + path + " not exists.");
        } catch (ClassNotFoundException e) {
            throw new RunningException(path + " is not a valid file");
        } catch (IOException e) {
            throw new RunningException(e.getMessage());
        }
        for(Map.Entry<String, Value> entry : newList.entrySet())
            make(entry.getKey(), entry.getValue());
//        if (output != null)
//            list.put(outputWordName, output);
//        if (test != null)
//            list.put(testWordName, test);
    }

    void clear() {
        Value output = null;
//        Value test = null;
        if (list.containsKey(outputWordName))
            output = list.get(outputWordName);
//        if (list.containsKey(testWordName))
//            test = list.get(testWordName);
        list.clear();
        if (output != null)
            list.put(outputWordName, output);
//        if (test != null)
//            list.put(testWordName, test);
    }

    void print() {
        for (Map.Entry<String, Value> entry : this) {
            String key = entry.getKey();
            switch (key) {
                case outputWordName:
//                case testWordName:
                    break;
                default:
                    System.out.println(key + " : " + entry.getValue());
            }
        }
    }

    boolean contains(String name) {
        return list.containsKey(name);
    }

    @Override
    public Iterator<Map.Entry<String, Value>> iterator() {
        return list.entrySet().iterator();
    }
}
