import React from 'react';
import {inject, observer} from 'mobx-react';
import Paper from 'material-ui/Paper';
import Tabs, {Tab} from 'material-ui/Tabs';
import {Link} from 'react-router-dom';
import Login from 'components/Account/Login';
import Register from 'components/Account/Register';

@inject('store')
@observer
export default class LoginRegister extends React.Component {

  render() {
    const {action} = this.props.match.params;

    return (
      <Paper>
        <Tabs
          value={action}
          indicatorColor="primary"
          textColor="primary"
          centered
        >
          <Tab value={"login"} label={"Login"} component={Link} to={"/login"}/>
          <Tab value={"register"} label={"Register"} component={Link} to={"/register"}/>
        </Tabs>
        {action === 'login' ? <Login/> : null}
        {action === 'register' ? <Register/> : null}
      </Paper>
    );
  }
}