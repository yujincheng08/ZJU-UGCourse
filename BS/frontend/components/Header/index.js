import React from 'react';
import {NavLink} from 'react-router-dom';
import List, {ListItem, ListItemText} from 'material-ui/List';

export default class Header extends React.Component {
  render() {
    return (
      <List>
        <nav id={"header"}>
          <ListItem component={NavLink} to={"/"} button>
            <ListItemText primary={"Home"}/>
          </ListItem>
          <ListItem component={NavLink} to={"/register"} button>
            <ListItemText primary={"Register"}/>
          </ListItem>
          <ListItem component={NavLink} to={"/login"} button>
            <ListItemText primary={"Login"}/>
          </ListItem>
        </nav>
      </List>
    );
  }
}