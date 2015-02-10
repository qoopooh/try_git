#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, urllib, json

CODE_TABLE = {
  '200':'Success',
  '400':'The transaction represented by this Purchase Token is no longer valid.',
  '496':'Invalid sharedSecret',
  '497':'Invalid User ID',
  '498':'Invalid Purchase Token',
  '499':'The Purchase Token was created with credentials that have expired, use renew to generate a valid purchase token.',
  '500':'There was an Internal Server Error'
}

# https://developer.amazon.com/public/apis/earn/in-app-purchasing/docs-v2/verifying-receipts-in-iap-2.0
developer_secret="2:smXBjZkWCxDMSBvQ8HBGsUS1PK3jvVc8tuTjLNfPHfYAga6WaDzXJPoWpfemXaHg:iEzHzPjJ-XwRdZ4b4e7Hxw=="
user_id="LRyD0FfW_3zeOlfJyxpVll-Z1rKn6dSf9xD3mUMSFg0="
purchase_token="wE1EG1gsEZI9q9UnI5YoZ2OxeoVKPdR5bvPMqyKQq5Y=:1:11"
server_url="https://appstore-sdk.amazon.com/version/1.0/verifyReceiptId/developer/{developerSecret}/user/{userId}/receiptId/{purchaseToken}".format(developerSecret=developer_secret, userId=user_id, purchaseToken=purchase_token)

print "REQUEST:", server_url

resp = urllib.urlopen(server_url)

code = resp.getcode()
print "\nRESPONSE:", code, "("+ CODE_TABLE[str(code)] + ")"

if code != 200:
  sys.exit(1)

data = json.loads(resp.read())

print "JSON:", json.dumps(data, indent=4, sort_keys=True)

