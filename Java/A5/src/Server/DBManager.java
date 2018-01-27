package Server;

import proto.ChatMessageProto.ChatMessage;
import proto.FriendListMessageProto.FriendListMessage;

import java.sql.*;
import java.util.ArrayList;

@SuppressWarnings({"SqlNoDataSourceInspection", "SqlDialectInspection"})
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
            "   FOREIGN KEY (userID) REFERENCES user (userID)," +
            "   FOREIGN KEY (friendID) REFERENCES user (userID)" +
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
            "   FOREIGN KEY (groupID) REFERENCES 'group' (groupID)," +
            "   FOREIGN KEY (userID) REFERENCES user (userID)" +
            ");" ,
            "CREATE TABLE IF NOT EXISTS userChatRecord" +
            "(" +
            "   fromID INTEGER NOT NULL," +
            "   toID INTEGER NOT NULL," +
            "   content TEXT NOT NULL," +
            "   timestamp INTEGER NOT NULL," +
            "   PRIMARY KEY (fromID, toID, timestamp)," +
            "   FOREIGN KEY (fromID) REFERENCES user (userID)," +
            "   FOREIGN KEY (toID) REFERENCES user (userID)" +
            ");",
            "CREATE TABLE IF NOT EXISTS groupChatRecord" +
            "(" +
            "   userID INTEGER NOT NULL," +
            "   groupID INTEGER NOT NULL," +
            "   content TEXT NOT NULL," +
            "   timestamp INTEGER NOT NULL," +
            "   PRIMARY KEY(userID, groupID, timestamp)," +
            "   FOREIGN KEY (userID) REFERENCES user (userID)," +
            "   FOREIGN KEY (groupID) REFERENCES 'group' (groupID)" +
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
            "SELECT fromID, content, timestamp " +
            "FROM userChatRecord " +
            "WHERE toID = ?;";
    private static final String removeUserChatRecordQuery = "" +
            "DELETE FROM userChatRecord " +
            "WHERE toID = ?";
    private static final String getGroupChatRecordQuery = "" +
            "SELECT groupID, content, timestamp " +
            "FROM groupChartRecord " +
            "WHERE userID = ?;";
    private static final String removeGroupChatRecordQuery = "" +
            "DELETE FROM groupChartRecord " +
            "WHERE userID = ?";
    private static final String friendListQuery = "" +
            "SELECT user.userID, nickName " +
            "FROM user, friend " +
            "WHERE user.userID = friend.friendID " +
            "AND friend.userID = ?;";
    private static final String groupListQeury = "" +
            "SELECT groupID, groupName " +
            "FROM user NATURAL JOIN 'group' " +
            "WHERE user.userID = ?;";
    private static final String addFriendQuery = "" +
            "INSERT INTO friend " +
            "(userID, friendID) " +
            "VALUES (?, ?), (?, ?);";
    private static final String addGroupQuery = "" +
            "INSERT INTO 'group' " +
            "(groupID, userID) " +
            "VALUES (?, ?);";
    private static final String getProfileQuery = "" +
            "SELECT userID, nickname " +
            "FROM user " +
            "WHERE userID = ?;";
    private static final String getGroupProfileQuery = "" +
            "SELECT groupID, groupName " +
            "FROM 'group' " +
            "WHERE gourpID=?;";
    private static final String setUserChatRecordQuery = "" +
            "INSERT INTO userChatRecord " +
            "(fromID, toID, content, timestamp) " +
            "VALUES (?, ?, ?, ?);";
    DBManager(String path) throws SQLException {
        connection = DriverManager.getConnection("jdbc:sqlite:" + path);
        Statement statement = connection.createStatement();
        for (String query: createTableQuery) {
            statement.execute(query);
        }
    }

    synchronized public long register(String password, String nickName, String email)
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

    synchronized public boolean login(long userID, String password)
            throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(loginQuery);
        preparedStatement.setLong(1, userID);
        preparedStatement.setString(2, password);
        ResultSet resultSet = preparedStatement.executeQuery();
        return resultSet.next() && resultSet.getBoolean(1);
    }

    synchronized public ArrayList<FriendListMessage> getFriendList(long userID) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(friendListQuery);
        preparedStatement.setLong(1, userID);
        ResultSet resultSet = preparedStatement.executeQuery();
        ArrayList<FriendListMessage> result = new ArrayList<>();
        while(resultSet.next()){
            long account  = resultSet.getLong(1);
            String nickName = resultSet.getString(2);
            FriendListMessage.Builder builder = FriendListMessage.newBuilder();
            builder.setType(FriendListMessage.Type.FRIEND);
            builder.setAccount(account);
            builder.setNickname(nickName);
            result.add(builder.build());
        }
        return result;
    }

    synchronized public ArrayList<FriendListMessage> getGourpList(long userID) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(groupListQeury);
        preparedStatement.setLong(1, userID);
        ResultSet resultSet = preparedStatement.executeQuery();
        ArrayList<FriendListMessage> result = new ArrayList<>();
        while(resultSet.next()){
            long groupID  = resultSet.getLong(1);
            String groupName = resultSet.getString(2);
            FriendListMessage.Builder builder = FriendListMessage.newBuilder();
            builder.setType(FriendListMessage.Type.GROUP);
            builder.setAccount(groupID);
            builder.setNickname(groupName);
            result.add(builder.build());
        }
        return result;
    }

    synchronized public FriendListMessage getProfile(long userID, FriendListMessage.Type type) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(getProfileQuery);
        preparedStatement.setLong(1, userID);
        ResultSet result = preparedStatement.executeQuery();
        if(result.next()) {
            FriendListMessage.Builder builder = FriendListMessage.newBuilder();
            builder.setType(type);
            builder.setAccount(result.getLong(1));
            builder.setNickname(result.getString(2));
            return builder.build();
        }
        return null;
    }

    synchronized public FriendListMessage getGroupProfile(long groupID) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(getGroupProfileQuery);
        preparedStatement.setLong(1, groupID);
        ResultSet result = preparedStatement.executeQuery();
        if(result.next()) {
            FriendListMessage.Builder builder = FriendListMessage.newBuilder();
            builder.setType(FriendListMessage.Type.GROUP);
            builder.setAccount(result.getLong(1));
            builder.setNickname(result.getString(2));
            return builder.build();
        }
        return null;
    }

    synchronized public FriendListMessage addFriend(long userID, long friendID) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(addFriendQuery);
        preparedStatement.setLong(1, userID);
        preparedStatement.setLong(2, friendID);
        preparedStatement.setLong(3, friendID);
        preparedStatement.setLong(4, userID);
        preparedStatement.execute();
        return getProfile(userID, FriendListMessage.Type.FRIEND);
    }


    synchronized public FriendListMessage addGroup(long userID, long groupID) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(addGroupQuery);
        preparedStatement.setLong(1, groupID);
        preparedStatement.setLong(2, userID);
        preparedStatement.execute();
        return getGroupProfile(groupID);
    }

    synchronized public boolean setUserChatRecord(long fromID, long toID, String content, long timestamp) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(setUserChatRecordQuery);
        preparedStatement.setLong(1, fromID);
        preparedStatement.setLong(2, toID);
        preparedStatement.setString(3, content);
        preparedStatement.setLong(4, timestamp);
        preparedStatement.execute();
        return true;
    }

    synchronized public ArrayList<ChatMessage> getUserChatRecord(long userID) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(getUserChatRecordQuery);
        preparedStatement.setLong(1, userID);
        ResultSet resultSet = preparedStatement.executeQuery();
        ArrayList<ChatMessage> result = new ArrayList<>();
        while(resultSet.next()){
            long fromID  = resultSet.getLong(1);
            String content = resultSet.getString(2);
            long timestamp = resultSet.getLong(3);
            ChatMessage.Builder builder = ChatMessage.newBuilder();
            builder.setTimestamp(timestamp);
            builder.setUserID(fromID);
            builder.setContent(content);
            result.add(builder.build());
        }
        return result;
    }

    synchronized public void resetChatRecord(long userID) throws SQLException {
        PreparedStatement preparedStatement = connection.prepareStatement(removeUserChatRecordQuery);
        preparedStatement.setLong(1, userID);
        preparedStatement.execute();
    }

}
