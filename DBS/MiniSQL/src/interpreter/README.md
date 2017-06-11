# Interpreter

## Function

Interpret user input and output the result.

## Definition

`Interpreter` can only be constructed by `Main` class because it take charged of `stdin` and `stdout`. So do not try to new an `Interpreter` again. The communication between `Interpreter` should be managed by `Main`.

`class Interpreter` provided four class to determined what had been token from user.

`class Action` is the main class to describe the action that should be executed by the database.

`class Column` describes the referenced columns or constant values of the action. For an insertion and conditions of the where clause, it contains the values of the insert constants with `type()==Column::Undefined`. `type()` between 1-225 means `char(type())`, `type()==Column::Int` means it's an integer, `type()==Column::Float` means it's a floating point number.

`class Constraint` describes the constraints when creating a new table.

`class Condition` is a grammar tree of all the conditions of where clause. If the condition node is `op()==Condition::Node`, then `value()` points to a `Column` object that describes the columns or the constant values. Judge the `type()` result of the Column object to determined whether it describes a column or a constant. Otherwise the `op()` of a condition node describes its operator between two operands: `firstOperand()` and `secondOperand()`, which are condition nodes as well.

## Usage

- Interfaces of `Interpreter`

  ```C++
  // A smart pointer. Use it like a normal pointer but do not have to free its space.
  template<typename T>
    using ptr = std::shared_ptr<T>;
  signals:
  // signals to emit when a parse is interpreted.
  void parsered(Action);
  public slots:
  //Display the results. If any error occurs during querying, also use this slot to display.
  void display(const QString &result);
  void display(const string &result);
  ```

- Interfaces of `Action`

  ```C++
  // All the type of actions 
  enum Type
  {
      Undefined, CreateTable, CreateIndex, Insert,
      Delete, DropIndex, Select, DropTable
  };
  //Return the type of the action
  auto actionType() const -> const Type &;
  //Return the list of all corresponding columns
  auto columns() const -> const ptr<const list<ptr<const Column>>>;
  //Return defined constraints.
  auto constraints() const -> const ptr<const Constraint>;
  //Return the list of all corresponding tables
  auto tableName() const -> const ptr<const list<ptr<const string>>>;
  //Return the corresponding index name
  auto indexName() const -> const ptr<const string>;
  //Return a grammar tree of all the conditons
  auto conditions() const -> const ptr<const Condition>;
  ```

- Interfaces of `Column`

  ```C++
  //Return the type of the column.
  auto type() const -> const Type &;
  //Return the name of the column or the value of a constant value.
  auto name() const -> const ptr<const string>;
  //Return the table name of the column.
  auto tableName() const -> const ptr<const string>;
  ```

- Interfaces of `Constraint`

  ```C++
  //Return the primary key column.
  auto primaryKey() const -> const ptr<const Column>;
  //Return a list of unique columns.
  auto uniques() const -> const ptr<list<ptr<const Column>>>;
  ```

- Interfaces of `Condition`

  ```C++
  //Type definition of conditon operator.
  enum Type
  {
    Underfined, Node, NotEqual, Equal,
    GreaterThan, LessThan, LessEqual,
    GreaterEqual, And, Or, Not
  };
  //Return the operator type.
  auto op() const -> const Type &;
  //Return its first operand.
  auto firstOperand() const -> const ptr<const Condition>;
  //Return its second operand.
  auto secondOperand() const -> const ptr<const Condition>;
  //Return the value of the node if op()==Node.
  auto value() const -> const ptr<const Column>;
  ```

## Attention

- `ptr` is a smart pointer and it use as normal pointer using `operator*()` and `operator->()`.
- If the `op()==Condition::Node` of a condition node, it only has the first operand and the second operand should be `nullptr`.
- Always detect some syntax errors that the interpreter cannot detect. Such as column's table is not mentioned error.