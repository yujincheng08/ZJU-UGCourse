import java.util.LinkedList;
import java.io.IOException;

import jline.console.ConsoleReader;
import jline.console.completer.AnsiStringsCompleter;

class WordStream
{
    private LinkedList<String> stream;
    private ConsoleReader consoleReader;
    private static AnsiStringsCompleter ansiStringsCompleter = new AnsiStringsCompleter(
            "make", "thing", "erase", "isname", "print",
            "read", "readlinst", "add", "sub", "mul", "div",
            "mod", "eq", "gt", "lt", "and", "or", "not", "repeat", "output", "stop");
    static final String DELIMITER = "\\s+|((?<=\\[)|(?=\\[))|((?<=])|(?=]))|((?<=\\())|((?=\\())|((?<=\\)))|((?=\\)))|(?=\\+)|(?=-)|((?<=\\*)|(?=\\*))|((?<=/)|(?=/))";
    //|((?<=\\+)|(?=\\+))|((?<=-)|(?=-))|((?<=\\*)|(?=\\*))|((?<=/)|(?=/))|((?<=%)|(?=%))";
    WordStream(String word)
    {
        this(word, true);
    }
    WordStream(String word, boolean readFromStdin)
    {
        this(readFromStdin);
        stream.offer(word);
    }
    WordStream(String [] words)
    {
        this(words, true);
    }
    WordStream(String [] words, boolean readFromStdin)
    {
        this(readFromStdin);
        for (String word: words)
            stream.offer(word);
    }

//    WordStream(String line)
//    {
//        this();
//        praseLine(line);
//    }

    WordStream(boolean readFromStdin)
    {
        stream = new LinkedList<>();
        if(readFromStdin)
            try {
                consoleReader = new ConsoleReader();
                consoleReader.setPrompt("MUA> ");
                consoleReader.addCompleter(ansiStringsCompleter);
            }catch (IOException e)
            {
                throw new RuntimeException("Error open terminal");
            }
    }

    WordStream()
    {
        this(true);
    }

    private void praseLine(String line)
    {
        if(line != null && line.length() > 0)
        {
            int indexOfComment = line.indexOf("//");
            if (indexOfComment > 0) line = line.substring(0, indexOfComment);
            String[] words = line.split(DELIMITER);
            for (String word : words)
                if(word.length() > 0)
                    stream.offer(word);
            //stream.offer("\n");
        }
    }

    private void readLine()
    {
        try
        {
            String line = consoleReader.readLine();
//            for (int s = 0, e = 0; s != line.length(); s = e)
//            {
//                while(e != line.length() && line.charAt(e++)!=' ');
//            }
            praseLine(line);

        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    String next()
    {
        String next = stream.poll();
        while(consoleReader != null && (next == null || next.equals("\n")) )
        {
            while (stream.isEmpty()) readLine();
            next = stream.poll();
        }
        return next;
    }

    WordStream merge(WordStream stream)
    {
        this.stream.addAll(stream.stream);
        return this;
    }

    WordStream putBack(String word)
    {
        stream.addFirst(word);
        return this;
    }
}
