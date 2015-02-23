package berm;

import java.util.regex.Pattern;

/**
* Validate the text value of the inout field against the rule set
* in the regular expression.
*/
public class RegexValidator {

  public static final String REGEX_SITE_MASTER_NAME = "^[a-zA-Z0-9]{1}[a-zA-Z0-9\\-\\@\\_\\.]{3,31}$";
  public static final String REGEX_PASSWORD = "^[a-zA-Z0-9\\!\\@\\#\\$\\%\\&\\*\\-\\_\\+\\=\\{\\}\\[\\]\\|\\~\\;\\:\\,\\.\\/\\?\\<\\>\\\n" +
      "\\u00C0-\\u017F]{6,32}$";
  public static final String REGEX_SITE_NAME = "^[a-z0-9]{1}[a-z-0-9\\-]{2,30}[a-z0-9]{1}$";
  public static final String REGEX_EMAIL = "[A-Za-z0-9!#$%&*+/=?^_`{|}~-\\u00C0-\\u017F]+(?:\\.[A-Za-z0-9!#$%&*+/=?^_`{|}~-\\u00C0-\\u017F]+)*@(?:[a-z0-9\\u00C0-\\u017F](?:[a-z0-9-\\u00C0-\\u017F]*[a-z0-9\\u00C0-\\u017F])?\\.)+[a-z0-9\\u00C0-\\u017F](?:[a-z0-9-\\u00C0-\\u017F]*[a-z0-9\\u00C0-\\u017F])?";

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

