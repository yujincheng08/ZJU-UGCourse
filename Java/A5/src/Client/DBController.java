package Client;

import java.sql.*;
import java.util.Vector;

@SuppressWarnings("SqlNoDataSourceInspection")
public class DBController {

    private Connection connection;
    static private final String checkTableQuery = "SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name=?";
    static private final String createTableQuery = "CREATE TABLE chatRecord (userID TEXT NOT NULL, content TEXT NOT NULL, timestamp INTEGER DEFAULT ( CAST(STRFTIME('%s', 'NOW') /60*60 * 1000 + STRFTIME('%f', 'NOW')*1000 AS INTEGER)) NOT NULL );";
    static private final String insertDefaultQuery = "INSERT INTO chatRecord (userID, content) VALUES (?,?);";
    static private final String insertQuery = "INSERT INTO chatRecord (userID, content, timestamp) VALUES (?,?,?);";
    static private final String getRecordQuery = "SELECT timestamp, content FROM chatRecord WHERE userID = ? ORDER BY timestamp DESC LIMIT ? OFFSET ?;";


    DBController(String path)
            throws SQLException {
        connection = DriverManager.getConnection("jdbc:sqlite:" + path);
    }

    public boolean checkTable(String tableName)
            throws SQLException {
        PreparedStatement statement = connection.prepareStatement(checkTableQuery);
        statement.setString(1, tableName);
        ResultSet result = statement.executeQuery();
        return result.getInt(1) > 0;
    }

    public boolean createChartRecordTable()
            throws SQLException {
        Statement statement = connection.createStatement();
        statement.execute(createTableQuery);
        return checkTable("chatRecord");
    }

    public boolean checkAndCreateChartRecordTable()
            throws SQLException {
        return checkTable("chatRecord") || createChartRecordTable();
    }

    public void storeChatRecord(String userID, String content)
            throws SQLException {
        checkAndCreateChartRecordTable();
        PreparedStatement statement = connection.prepareStatement(insertDefaultQuery);
        statement.setString(1, userID);
        statement.setString(2, content);
        statement.execute();
    }

    public void storeChatRecord(String userID, ChatRecord chatRecord)
            throws SQLException {
        checkAndCreateChartRecordTable();
        PreparedStatement statement = connection.prepareStatement(insertQuery);
        statement.setString(1, userID);
        statement.setString(2, chatRecord.getContent());
        statement.setLong(3, chatRecord.getTimestamp());
        statement.execute();
    }

    public Vector<ChatRecord> getChartRecord(String userID)
            throws SQLException {
        return getChartRecord(userID, 0);
    }

    public Vector<ChatRecord> getChartRecord(String userID, int offset)
            throws SQLException {
        return getChartRecord(userID, 30, offset);
    }

    public Vector<ChatRecord> getChartRecord(String userID, int limit, int offset)
            throws SQLException {
        Vector<ChatRecord> ChatRecordList = new Vector<>();
        checkAndCreateChartRecordTable();
        PreparedStatement statement = connection.prepareStatement(getRecordQuery);
        statement.setString(1, userID);
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
            dbController.storeChatRecord("1", "Hello!");
            Vector<ChatRecord> records = dbController.getChartRecord("1", 10, 1);
            for (ChatRecord record : records)
                System.out.println(record);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

}
