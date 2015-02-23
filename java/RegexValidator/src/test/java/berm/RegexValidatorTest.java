package berm;

import org.testng.annotations.*;
import static org.testng.AssertJUnit.*;

public class RegexValidatorTest {

  RegexValidator mSiteName;

  @BeforeClass
  public void setUp() {
    mSiteName = new RegexValidator(RegexValidator.REGEX_SITE_NAME);
  }

  @Test
  public void simpleSite() {
    assertEquals(mSiteName.isFieldValid("berm"), true);
  }
}

