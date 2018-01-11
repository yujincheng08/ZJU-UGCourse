import java.io.*;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Stack;
import java.util.Map;

class WordList implements Iterable<Map.Entry<String, Value>>{
    static final String outputWordName = "0";
    // static final String testWordName = "1";
    private Stack<HashMap<String, Value>> list;
    private HashMap<String, Value> root;
    WordList() {
        root = new HashMap<>();
        list = new Stack<>();
        list.push(root);
    }

    public void newSpace() {
        list.push(new HashMap<>());
    }

    public void endSpace() {
        list.pop();
    }

    private HashMap<String, Value> current() {
        return list.peek();
    }

    public void export(String name, Value value) {
        root.put(name, value);
    }

    Value thing(String name) throws RunningException {
        Value value = null;
        for(int i = list.size() - 1; i>=0; --i){
            value = list.get(i).get(name);
            if(value != null)
                break;
        }
        if (value == null)
            throw new RunningException("Word " + name + " does not exist.");
        else
            return value;
    }

    void make(String name, Value value) {
        current().put(name, value);
    }

    void make(String name, WordStream stream)
            throws RunningException, SyntaxException {
        Value value = current().get(name);
        //word = new Word();
        Value newValue = Interpreter.value(stream);
        if (value != null) {
            current().remove(name);
        }
        current().put(name, newValue);
    }

    Value isname(String name) {
        if (current().containsKey(name))
            return Value.TRUE;
        else
            return Value.FALSE;
    }

    void erase(String name)
            throws RunningException {
        if (current().containsKey(name))
            current().remove(name);
        else
            throw new RunningException("Word " + name + "does not exist.");
    }

    void save(String path)
            throws RunningException {
        Value output = null;
//        Value test = null;
        if (current().containsKey(outputWordName)) {
            output = current().get(outputWordName);
            current().remove(outputWordName);
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
            objectOutputStream.writeObject(current());
        } catch (FileNotFoundException e) {
            throw new RunningException("File " + path + " not exists.");
        } catch (IOException e) {
            throw new RunningException(e.toString());
        } finally {
            if (output != null)
                current().put(outputWordName, output);
//            if (test != null)
//                list.put(testWordName, test);
        }
    }

    void load(String path)
            throws RunningException {
        File file = new File(path);
        HashMap<String, Value> newList;
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
        if (current().containsKey(outputWordName))
            output = current().get(outputWordName);
//        if (list.containsKey(testWordName))
//            test = list.get(testWordName);
        current().clear();
        if (output != null)
            current().put(outputWordName, output);
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

    Value getOutput()
    {
        return current().get(outputWordName);
    }

    boolean contains(String name) {
        for (int i = list.size() - 1; i >= 0; i--) {
            if(list.get(i).containsKey(name))
                return true;
        }
        return false;
    }

    @Override
    public Iterator<Map.Entry<String, Value>> iterator() {
        return current().entrySet().iterator();
    }
}
