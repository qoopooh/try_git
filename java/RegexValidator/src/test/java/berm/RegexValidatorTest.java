package berm;

import org.testng.annotations.*;
import static org.testng.AssertJUnit.*;

public class RegexValidatorTest {

  RegexValidator mUserName;
  RegexValidator mSiteName;

  @BeforeClass
  public void setUp() {
    mUserName = new RegexValidator(RegexValidator.REGEX_SITE_MASTER_NAME);
    mSiteName = new RegexValidator(RegexValidator.REGEX_SITE_NAME);
  }

  @Test
  public void simpleSite() {
    assertEquals(mUserName.isFieldValid("berm"), true);
    assertEquals(mSiteName.isFieldValid("berm"), true);
  }

  @Test
  public void startHyphen() {
    assertEquals(mUserName.isFieldValid("-erm"), false);
    assertEquals(mSiteName.isFieldValid("-erm"), false);
  }

  @Test
  public void midHyphen() {
    assertEquals(mUserName.isFieldValid("be-m"), true);
    assertEquals(mSiteName.isFieldValid("b-rm"), true);
  }

  @Test
  public void endHyphen() {
    assertEquals(mUserName.isFieldValid("ber-"), true);
    assertEquals(mSiteName.isFieldValid("ber-"), false);
  }

  @Test
  public void startCapital() {
    assertEquals(mUserName.isFieldValid("Berm"), true);
    assertEquals(mSiteName.isFieldValid("Berm"), false);
  }

  @Test
  public void midCapital() {
    assertEquals(mUserName.isFieldValid("bERm"), true);
    assertEquals(mSiteName.isFieldValid("beRm"), false);
  }

  @Test
  public void startNumber() {
    assertEquals(mUserName.isFieldValid("20Century"), true);
    assertEquals(mSiteName.isFieldValid("7erm"), true);
  }

  @Test
  public void midUnderscore() {
    assertEquals(mUserName.isFieldValid("middle_with_underscore"), true);
    assertEquals(mSiteName.isFieldValid("w_in"), false);
  }

  @Test
  public void name3Char() {
    assertEquals(mUserName.isFieldValid("ber"), false);
    assertEquals(mSiteName.isFieldValid("ber"), false);
  }

  @Test
  public void name20Char() {
    assertEquals(mUserName.isFieldValid("This5houldNot3eProb."), true);
    assertEquals(mSiteName.isFieldValid("1234567890-123456789"), true);
  }

  @Test
  public void name32Char() {
    assertEquals(mUserName.isFieldValid("PhuchitKan-JaNaKhan@Yahoo.com.1."), true);
    assertEquals(mSiteName.isFieldValid("1234567890-123456789-12345678912"), true);
  }

  @Test
  public void name33Char() {
    assertEquals(mUserName.isFieldValid("1234567890-123456789-123456789-12"), false);
    assertEquals(mSiteName.isFieldValid("1234567890-123456789-123456789-12"), false);
  }
}

