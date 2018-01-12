import java.util.Arrays;
import java.util.HashSet;
import java.util.LinkedList;
import java.io.IOException;

import jline.console.ConsoleReader;
import jline.console.completer.AnsiStringsCompleter;

class WordStream {
    private LinkedList<String> stream;
    private ConsoleReader consoleReader;
    static HashSet<String> keyWrods= new HashSet<>(
            Arrays.asList("make", "thing", "erase", "isname", "print",
                    "read", "readlinst", "add", "sub", "mul", "div",
                    "mod", "eq", "gt", "lt", "and", "or", "not", "repeat", "output", "stop", "if",
                    "random", "sqrt", "isnumber", "isbool", "islist", "isempty", "word", "list",
                    "sentence", "join", "first", "last", "butlast", "butfirst", "item", "int", "return",
                    "wait", "save", "load", "erall", "poall", "run", "export", "abs"));
    private static AnsiStringsCompleter ansiStringsCompleter = new AnsiStringsCompleter(keyWrods);

    static private final String DELIMITER = "\\s+|((?<=\\[)|(?=\\[))|((?<=])|(?=]))";

    //|((?<=\\())|((?=\\())|((?<=\\)))|((?=\\)))|(?=\\+)|(?=-)|((?<=\\*)|(?=\\*))|((?<=/)|(?=/))";
    //|((?<=\\+)|(?=\\+))|((?<=-)|(?=-))|((?<=\\*)|(?=\\*))|((?<=/)|(?=/))|((?<=%)|(?=%))";
    WordStream(String word, boolean readFromStdin) {
        this(readFromStdin);
        stream.offer(word);
    }

    @SuppressWarnings("unused")
    WordStream(String[] words) {
        this(words, true);
    }

    @SuppressWarnings("WeakerAccess")
    WordStream(String[] words, boolean readFromStdin) {
        this(readFromStdin);
        for (String word : words)
            stream.offer(word);
    }

//    WordStream(String line)
//    {
//        this();
//        praseLine(line);
//    }

    WordStream(boolean readFromStdin) {
        stream = new LinkedList<>();
        if (readFromStdin)
            try {
                consoleReader = new ConsoleReader();
                consoleReader.setPrompt("MUA> ");
                consoleReader.addCompleter(ansiStringsCompleter);
            } catch (IOException e) {
                throw new RuntimeException("Error open terminal");
            }
    }

    WordStream() {
        this(true);
    }

    private void praseLine(String line) {
        if (line != null && line.length() > 0) {
            int indexOfComment = line.indexOf("//");
            if (indexOfComment > 0) line = line.substring(0, indexOfComment);
            String[] words = line.split(DELIMITER);
            for (String word : words)
                if (word.length() > 0)
                    stream.offer(word);
            //stream.offer("\n");
        }
    }

    private void readLine() {
        try {
            String line = consoleReader.readLine();
//            for (int s = 0, e = 0; s != line.length(); s = e)
//            {
//                while(e != line.length() && line.charAt(e++)!=' ');
//            }
            praseLine(line);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    String next(boolean keep) {
        String next = stream.poll();
        while (consoleReader != null && (next == null || next.equals("\n"))) {
            while (stream.isEmpty()) readLine();
            next = stream.poll();
        }
        if (keep || next == null)
            return next;
        else {
            String[] list = next.split("((?<=\\())|((?=\\())|((?<=\\)))|((?=\\)))|(?=\\+)|(?=-)|((?<=\\*)|(?=\\*))|((?<=/)|(?=/))|(?=<)|(?<=<)(?=[^=])|(?=>)|(?<=>)(?=[^=])|(?<=[^<>])(?==)|(?<==)");
            for (int i = list.length - 1; i > 0; --i)
                stream.push(list[i]);
            return list[0];
        }
    }

    String next() {
        return next(false);
    }

    @SuppressWarnings("UnusedReturnValue")
    WordStream merge(WordStream stream) {
        this.stream.addAll(stream.stream);
        return this;
    }

    @SuppressWarnings("UnusedReturnValue")
    WordStream merge(String word)
    {
        this.stream.add(word);
        return this;
    }

    WordStream putBack(String word) {
        stream.addFirst(word);
        return this;
    }

    @SuppressWarnings("UnusedReturnValue")
    WordStream putBack(WordStream stream) {
        //noinspection unchecked
        LinkedList<String> tmp = (LinkedList<String>) stream.stream.clone();
        tmp.addAll(this.stream);
        this.stream = tmp;
        return this;
    }

    @SuppressWarnings("UnusedReturnValue")
    WordStream replace(WordStream stream) {
        if(consoleReader == null) {
            this.stream = stream.stream;
        }
        return this;
    }
}
