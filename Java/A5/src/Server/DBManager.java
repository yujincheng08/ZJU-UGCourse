package Server;

import java.sql.*;

@SuppressWarnings("SqlNoDataSourceInspection")
public class DBManager {
    private Connection connection;
    private static final String [] createTableQuery = new String[]{"" +
            "CREATE TABLE IF NOT EXISTS user" +
            "(" +
            "   userID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," +
            "   password TEXT NOT NULL," +
            "   nickName TEXT NOT NULL," +
            "   email TEXT NOT NULL" +
            ");",
            "CREATE TABLE IF NOT EXISTS friend" +
            "(" +
            "   userID INTEGER NOT NULL," +
            "   friendID INTEGER NOT NULL," +
            "   PRIMARY KEY (userID, friendID)," +
            "   FOREIGN KEY (userID) REFERENCES user," +
            "   FOREIGN KEY (userID) REFERENCES user" +
            ");" ,
            "CREATE TABLE IF NOT EXISTS 'group'" +
            "(" +
            "   groupID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," +
            "   groupName TEXT NOT NULL" +
            ");" ,
            "CREATE TABLE IF NOT EXISTS groupMember" +
            "(" +
            "   groupID INTEGER NOT NULL," +
            "   userID INTEGER NOT NULL," +
            "   PRIMARY KEY (groupID, userID)," +
            "   FOREIGN KEY (groupID) REFERENCES 'group'," +
            "   FOREIGN KEY (userID) REFERENCES user" +
            ");" ,
            "CREATE TABLE IF NOT EXISTS userChatRecord" +
            "(" +
            "   fromID INTEGER NOT NULL," +
            "   toID INTEGER NOT NULL," +
            "   content TEXT NOT NULL," +
            "   timestamp INTEGER NOT NULL," +
            "   PRIMARY KEY (fromID, toID, timestamp)," +
            "   FOREIGN KEY (fromID) REFERENCES user," +
            "   FOREIGN KEY (toID) REFERENCES user" +
            ");",
            "CREATE TABLE IF NOT EXISTS groupChatRecord" +
            "(" +
            "   userID INTEGER NOT NULL," +
            "   groupID INTEGER NOT NULL," +
            "   content TEXT NOT NULL," +
            "   timestamp INTEGER NOT NULL," +
            "   PRIMARY KEY(userID, groupID, timestamp)," +
            "   FOREIGN KEY (userID) REFERENCES user," +
            "   FOREIGN KEY (groupID) REFERENCES 'group'" +
            ")"};
    private static final String loginQuery = "" +
            "SELECT EXISTS" +
            "(" +
            "   SELECT * " +
            "   FROM user " +
            "   WHERE userID = ? AND password = ?" +
            ");";
    private static final String registerQuery = "" +
            "INSERT INTO user " +
            "(password, nickname, email) " +
            "VALUES (?, ?, ?)";
    private static final String getUserChatRecordQuery = "" +
            "SELECT toID, content, timestamp " +
            "FROM userChatRecord " +
            "WHERE fromID = ?;";
    private static final String removeUserChatRecordQuery = "" +
            "DELETE FROM userChatRecord " +
            "WHERE fromID = ?";
    private static final String getGroupChatRecordQuery = "" +
            "SELECT groupID, content, timestamp " +
            "FROM groupChartRecord " +
            "WHERE userID = ?;";
    private static final String removeGroupChatRecordQuery = "" +
            "DELETE FROM groupChartRecord " +
            "WHERE userID = ?";

    DBManager(String path) throws SQLException {
        connection = DriverManager.getConnection("jdbc:sqlite:" + path);
        Statement statement = connection.createStatement();
        for (String query: createTableQuery) {
            statement.execute(query);
        }
        //statement.execute(getUserChatRecordQuery);
        //statement.execute(removeUserChatRecordQuery);
        //statement.execute(getGroupChatRecordQuery);
        //statement.execute(removeGroupChatRecordQuery);
    }

    public long register(String password, String nickName, String email)
            throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(registerQuery, Statement.RETURN_GENERATED_KEYS);
        preparedStatement.setString(1, password);
        preparedStatement.setString(2, nickName);
        preparedStatement.setString(3, email);
        int affectRows = preparedStatement.executeUpdate();
        if (affectRows == 0)
            throw new SQLException("Register failed.");
        try (ResultSet resultSet = preparedStatement.getGeneratedKeys()) {
            if (resultSet.next())
                return resultSet.getLong(1);
            else throw new SQLException("Register failed");
        }
    }

    public boolean login(long userID, String password)
            throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(loginQuery);
        preparedStatement.setLong(1, userID);
        preparedStatement.setString(2, password);
        ResultSet resultSet = preparedStatement.executeQuery();
        return resultSet.next() && resultSet.getBoolean(1);
    }
}
