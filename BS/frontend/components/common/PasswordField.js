import React from 'react';
import Input, {InputAdornment, InputLabel} from 'material-ui/Input';
import {FormControl} from 'material-ui/Form';
import IconButton from 'material-ui/IconButton'
import Visibility from 'material-ui-icons/Visibility';
import VisibilityOff from 'material-ui-icons/VisibilityOff';

export default class PasswordField extends React.Component {

  componentWillMount() {
    this.setState({showPassword: false});
  }

  onShowPassword = () => {
    this.setState({showPassword: !this.state.showPassword});
  };

  render() {
    const {showPassword} = this.state;
    const {label, id} = this.props;
    return (
      <FormControl {...this.props}>
        <InputLabel htmlFor={id ? id : label}>{label}</InputLabel>
        <Input
          id={id ? id : label}
          type={showPassword ? "text" : "password"}
          endAdornment={
            <InputAdornment position={"end"}>
              <IconButton
                aria-label={"Toggle password visibility"}
                onClick={this.onShowPassword}
              >
                {this.state.showPassword ? <Visibility/> : <VisibilityOff/>}
              </IconButton>
            </InputAdornment>}
        >
          {this.props.children}
        </Input>
      </FormControl>
    );
  }
}