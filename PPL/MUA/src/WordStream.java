import java.util.LinkedList;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class WordStream
{
    private LinkedList<String> stream;
    private BufferedReader bufferedReader;
    WordStream(String [] words)
    {
        this();
        for (String word: words)
            stream.offer(word);
    }

    WordStream(String word)
    {
        this();
        stream.offer(word);
    }

    WordStream()
    {
        stream = new LinkedList<>();
        bufferedReader = new BufferedReader(new InputStreamReader(System.in));
    }

    private void readLine()
    {
        try
        {
            String line = bufferedReader.readLine();
//            for (int s = 0, e = 0; s != line.length(); s = e)
//            {
//                while(e != line.length() && line.charAt(e++)!=' ');
//            }
            if(line != null && line.length() > 0)
            {
                int indexOfComment = line.indexOf("//");
                if(indexOfComment>0)
                    line = line.substring(0, indexOfComment);
                String []words = line.split("\\s+");
                for(String word : words)
                        stream.offer(word);
                //stream.offer("\n");
            }
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    public String next()
    {
        String next = null;
        while(next == null || next.equals("\n"))
        {
            while (stream.isEmpty()) readLine();
            next = stream.poll();
        }
        return next;
    }
}
