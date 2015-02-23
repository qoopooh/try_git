package berm;

import java.util.regex.Pattern;

/**
* Validate the text value of the inout field against the rule set
* in the regular expression.
*/
public class RegexValidator {
  private String mRegex;

  /**
  * Instantiates a new regex validator.
  * 
  * @param regex
  *          the regex
  */
  public RegexValidator(String regex) {
    mRegex = regex;
  }

  /**
  * Return true if the text contain in the editText matches the regular
  * expression.
  * 
  * @return true, if is field valid
  */
  protected boolean isFieldValid(String value)
  {
    return Pattern.matches(mRegex, value);
  }
}

