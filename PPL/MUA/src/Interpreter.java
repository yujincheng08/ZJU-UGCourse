public class Interpreter {
    static WordList wordList = new WordList();

    public static void main(String[] args) {
        WordStream wordStream = new WordStream();
        wordList.make("pi", new Word("3.14159"));
        //noinspection InfiniteLoopStatement
        while (true) {
            try {
                interpret(wordStream);
            } catch (StackOverflowError e) {
                System.err.println("StackOverflow");
            }
        }

    }


    private static boolean isWordLiterary(String name) {
        return name.startsWith("\"");
    }

    private static Value getValue(String operator, WordStream stream)
            throws RunningException, SyntaxException {
        switch (operator) {
            case "thing":
                return Function.thing(stream);
            case "isname":
                return Function.isName(stream);
            case "read":
                return Function.read(stream);
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
            case "random":
                return Function.random(stream);
            case "sqrt":
                return Function.sqrt(stream);
            case "isnumber":
                return Function.isnumber(stream);
            case "isbool":
                return Function.isbool(stream);
            case "islist":
                return Function.islist(stream);
            case "isempty":
                return Function.isempty(stream);
            case "word":
                return Function.word(stream);
            case "list":
                return Function.list(stream);
            case "sentence":
                return Function.sentence(stream);
            case "join":
                return Function.join(stream);
            case "first":
                return Function.first(stream);
            case "last":
                return Function.last(stream);
            case "butfirst":
                return Function.butfirst(stream);
            case "butlast":
                return Function.butlast(stream);
            case "item":
                return Function.item(stream);
            case "int":
                return Function.Int(stream);
            case "abs":
                return Function.abs(stream);
            default:
                if (operator.startsWith(":"))
                    return Function.thing(operator.substring(1));
                else if (isWordLiterary(operator))
                    return new Word(operator.substring(1));
                else if (operator.matches("[+-]?\\d+(\\.\\d+)?"))
                    return new Word(operator);
                else if (operator.equals("["))
                    return new List(stream);
                else if (operator.startsWith("("))
                    return new Word(stream.putBack(operator));
                else
                    return Function.run(operator, stream);
        }
    }

    static Value value(WordStream stream)
            throws RunningException, SyntaxException {
        String operator = stream.next();
        if (operator == null)
            throw new RunningException("Expected a value but got null");
        Value val = getValue(operator, stream);
        if (val == null)
            throw new RunningException(operator + " does not return a value");
        return val;
    }

    private static void run(WordStream stream, Value list) {
        System.out.println("Running: " + list.toString());
        stream.putBack(list.toWordStream());
    }

    private static void printResult(WordStream stream)
            throws RunningException, SyntaxException {
        String operator = stream.next();
        if (operator == null)
            throw new RunningException("Expected a value but got null");
        if (operator.equals("[")) run(stream, new List(stream));
        else if (operator.startsWith(":"))
            run(stream, Function.thing(operator.substring(1)));
        else {
            Value val = getValue(operator, stream); //Function calls
            if (val != null) {
                if (val.isList())
                    run(stream, val);
                else System.out.println(val.toString());
            }
        }
    }

    static void interpret(WordStream stream) {
        mainLoop:
        while (true) {
            String command = stream.next();
            try {
                if (command == null) {
                    break;
                }
                switch (command) {
                    case "make":
                        Function.make(stream);
                        break;
                    case "print":
                        Function.print(stream);
                        break;
                    case "erase":
                        Function.erase(stream);
                        break;
                    case "thing":
                        interpret(Function.thing(stream).toWordStream());
                        break;
                    case "output":
                        Function.output(stream);
                        break;
                    case "repeat":
                        Function.repeat(stream);
                        break;
                    case "wait":
                        Function.wait(stream);
                        break;
                    case "save":
                        Function.save(stream);
                        break;
                    case "load":
                        Function.load(stream);
                        break;
                    case "erall":
                        Function.erall();
                        break;
                    case "poall":
                        Function.poall();
                        break;
                    case "if":
                        Function.If(stream);
                        break;
                    case "run":
                        Function.run(stream);
                        break;
                    case "export":
                        Function.export();
                        break;
                    case "return":
                        if (!Function.Return(stream))
                            break;
                    case "stop":
                        break mainLoop;
                    default:
                        printResult(stream.putBack(command));
                }
            } catch (RunningException e) {
                System.out.println("Runtime Error: " + e.getMessage());
            } catch (SyntaxException e) {
                System.out.println("Syntax Error: " + e.getMessage());
            }
        }
    }
}
