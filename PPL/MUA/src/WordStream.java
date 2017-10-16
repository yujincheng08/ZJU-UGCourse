import java.util.LinkedList;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

class WordStream
{
    private LinkedList<String> stream;
    private BufferedReader bufferedReader;
    WordStream(String [] words)
    {
        this();
        for (String word: words)
            stream.offer(word);
    }

//    WordStream(String line)
//    {
//        this();
//        praseLine(line);
//    }

    WordStream()
    {
        stream = new LinkedList<>();
        bufferedReader = new BufferedReader(new InputStreamReader(System.in));
    }

    private void praseLine(String line)
    {
        if(line != null && line.length() > 0)
        {
            int indexOfComment = line.indexOf("//");
            if (indexOfComment > 0) line = line.substring(0, indexOfComment);
            String[] words = line.split("\\s+");
            for (String word : words)
                stream.offer(word);
            //stream.offer("\n");
        }
    }

    private void readLine()
    {
        System.out.print("> ");
        try
        {
            String line = bufferedReader.readLine();
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
        String next = null;
        while(next == null || next.equals("\n"))
        {
            while (stream.isEmpty()) readLine();
            next = stream.poll();
        }
        return next;
    }

    void merge(WordStream stream)
    {
        this.stream.addAll(stream.stream);
    }
}
