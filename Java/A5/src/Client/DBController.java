package Client;

import java.sql.*;
import java.util.Vector;

@SuppressWarnings("SqlNoDataSourceInspection")
public class DBController {

    private Connection connection;
    static private final String createTableQuery = "" +
            "CREATE TABLE IF NOT EXISTS chatRecord " +
            "(" +
            "   userID INTEGER NOT NULL, " +
            "   content TEXT NOT NULL, " +
            "   timestamp INTEGER " +
            "       DEFAULT ( CAST(STRFTIME('%s', 'NOW') /60*60 * 1000 + STRFTIME('%f', 'NOW')*1000 AS INTEGER)) NOT NULL, " +
            "   PRIMARY KEY (userID, timestamp)" +
            ");";
    static private final String insertDefaultQuery = "" +
            "INSERT INTO chatRecord " +
            "(userID, content) VALUES (?,?);";
    static private final String insertQuery =
            "INSERT INTO chatRecord " +
            "(userID, content, timestamp) VALUES (?,?,?);";
    static private final String getRecordQuery = "" +
            "SELECT timestamp, content " +
            "FROM chatRecord " +
            "WHERE userID = ? " +
            "ORDER BY timestamp DESC " +
            "LIMIT ? OFFSET ?;";


    DBController(String path)
            throws SQLException {
        connection = DriverManager.getConnection("jdbc:sqlite:" + path);
        Statement statement = connection.createStatement();
        statement.execute(createTableQuery);
    }

    public void storeChatRecord(long userID, String content)
            throws SQLException {
        PreparedStatement statement = connection.prepareStatement(insertDefaultQuery);
        statement.setLong(1, userID);
        statement.setString(2, content);
        statement.execute();
    }

    public void storeChatRecord(long userID, ChatRecord chatRecord)
            throws SQLException {
        PreparedStatement statement = connection.prepareStatement(insertQuery);
        statement.setLong(1, userID);
        statement.setString(2, chatRecord.getContent());
        statement.setLong(3, chatRecord.getTimestamp());
        statement.execute();
    }

    public Vector<ChatRecord> getChartRecord(long userID)
            throws SQLException {
        return getChartRecord(userID, 0);
    }

    public Vector<ChatRecord> getChartRecord(long userID, int offset)
            throws SQLException {
        return getChartRecord(userID, 30, offset);
    }

    public Vector<ChatRecord> getChartRecord(long userID, int limit, int offset)
            throws SQLException {
        Vector<ChatRecord> ChatRecordList = new Vector<>();
        PreparedStatement statement = connection.prepareStatement(getRecordQuery);
        statement.setLong(1, userID);
        statement.setInt(2, limit);
        statement.setInt(3, offset);
        ResultSet result = statement.executeQuery();
        while (result.next()) {
            ChatRecordList.add(new ChatRecord(result.getLong("timestamp"), result.getString("content")));
        }
        return ChatRecordList;
    }

    public static void main(String[] arg) {
        try {
            DBController dbController = new DBController("./db.db");
            dbController.storeChatRecord(1L, "Hello!");
            Vector<ChatRecord> records = dbController.getChartRecord(1L, 10, 1);
            for (ChatRecord record : records)
                System.out.println(record);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

}
