
public class Interpreter
{
    private static WordStream wordStream;
    public static void main(String [] args)
    {
        wordStream = new WordStream();
        interpret(wordStream);
    }

    public static Value value(WordStream stream)
    {
        String operate = stream.next();
        try
        {
            switch (operate)
            {
                case "thing":
                    return Function.thing(stream);
                case "isname":
                    return Function.isName(stream);
                case "read":
                    return Function.read();
                case "readlinst":
                    return Function.readLinst();
                case "add":
                    return Function.add(stream);
                case "sub":
                    return Function.sub(stream);
                case "mul":
                    return Function.mul(stream);
                case "div":
                    return Function.div(stream);
                case "mod":
                    return Function.mod(stream);
                case "eq":
                    return Function.eq(stream);
                case "gt":
                    return Function.gt(stream);
                case "lt":
                    return Function.lt(stream);
                case "and":
                    return Function.and(stream);
                case "or":
                    return Function.or(stream);
                case "not":
                    return Function.not(stream);
                default:
            }
        }
        catch(SyntaxException e)
        {
            e.printStackTrace();
        }
        catch(RunningException e)
        {
            e.printStackTrace();
        }
        finally
        {

        }
        return new Value();
    }

    public static void interpret(WordStream stream)
    {
        String command = stream.next();
        switch(command)
        {
            case "make":
                Function.make(stream);
                break;
            case "print":
                Function.print(stream);
                break;
            case "erase":
                Function.erase(stream);
                break;
//            case "if":
//                break;
//            case "run":
//                break;
            default:
                break;
        }
    }
}
