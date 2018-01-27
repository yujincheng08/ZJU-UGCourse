package Client;

import proto.ChatMessageProto.ChatMessage;

import java.sql.*;
import java.util.ArrayList;

@SuppressWarnings({"SqlNoDataSourceInspection", "SqlResolve", "SqlDialectInspection"})
public class DBController {

    private Connection connection;
    static private final String [] createTableQuery = new String[]{"" +
            "CREATE TABLE IF NOT EXISTS userChatRecord " +
            "(" +
            "   fromID INTEGER NOT NULL, " +
            "   toID INTEGER NOT NULL, " +
            "   content TEXT NOT NULL, " +
            "   timestamp INTEGER " +
            "       DEFAULT ( CAST(STRFTIME('%s', 'NOW') /60*60 * 1000 + STRFTIME('%f', 'NOW')*1000 AS INTEGER)) NOT NULL, " +
            "   PRIMARY KEY (fromID, toID, timestamp)" +
            ");",
            "CREATE TABLE IF NOT EXISTS grouptChatRecord" +
            "(" +
            "   groupID INTEGER NOT NULL," +
            "   userID INTEGER NOT NULL," +
            "   timestamp INTEGER NOT NULL," +
            "   content TEXT NOT NULL," +
            "   PRIMARY KEY (groupID, userID, timestamp)" +
            ");"
    };
    static private final String insertDefaultQuery = "" +
            "INSERT INTO userChatRecord " +
            "(fromID, toID, content) VALUES (?,?,?);";
    static private final String insertQuery =
            "INSERT INTO userChatRecord " +
            "(fromID, toID, content, timestamp) VALUES (?,?,?,?);";
    static private final String getRecordQuery = "" +
            "SELECT fromID, timestamp, content " +
            "FROM userChatRecord " +
            "WHERE fromID = ? " +
            "OR toID = ?" +
            "ORDER BY timestamp DESC " +
            "LIMIT ? OFFSET ?;";


    DBController(String path)
            throws SQLException {
        connection = DriverManager.getConnection("jdbc:sqlite:" + path);
        Statement statement = connection.createStatement();
        for(String query : createTableQuery)
            statement.execute(query);
    }

    public void storeChatRecord(long userID, ChatMessage chatMessage)
            throws SQLException {
        PreparedStatement statement = connection.prepareStatement(insertQuery);
        statement.setLong(1, chatMessage.getUserID());
        statement.setLong(2, userID);
        statement.setString(3, chatMessage.getContent());
        statement.setLong(4, chatMessage.getTimestamp());
        statement.execute();
    }

    public void storeSendRecord(long userID, ChatMessage chatMessage) throws SQLException {
        PreparedStatement statement = connection.prepareStatement(insertQuery);
        statement.setLong(1, userID);
        statement.setLong(2, chatMessage.getUserID());
        statement.setString(3, chatMessage.getContent());
        statement.setLong(4, chatMessage.getTimestamp());
        statement.execute();
    }

    public Iterable<ChatMessage> getChartRecord(long userID)
            throws SQLException {
        return getChartRecord(userID, 0);
    }

    public Iterable<ChatMessage> getChartRecord(long userID, int offset)
            throws SQLException {
        return getChartRecord(userID, 30, offset);
    }

    public ArrayList<ChatMessage> getChartRecord(long userID, int limit, int offset)
            throws SQLException {
        ArrayList<ChatMessage> ChatRecordList = new ArrayList<>();
        PreparedStatement statement = connection.prepareStatement(getRecordQuery);
        statement.setLong(1, userID);
        statement.setLong(2, userID);
        statement.setInt(3, limit);
        statement.setInt(4, offset);
        ResultSet result = statement.executeQuery();
        while (result.next()) {
            ChatRecordList.add(
                    ChatMessage.newBuilder().setUserID(result.getLong("fromID"))
                            .setTimestamp(result.getLong("timestamp"))
                    .setContent(result.getString("content")).build());
        }
        return ChatRecordList;
    }
}
