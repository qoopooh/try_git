# Create your views here.
from django.http import HttpResponse
import pyodbc
import json
import collections

def index(request):
  return HttpResponse("Hello, world. You're at the polls index.")

def detail(request, question_id):
  cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=ERPTEST\\ERP;DATABASE=GeoRiotinto;UID=sa;PWD=sa')
  cursor = cnxn.cursor();
  cursor.execute("""
      SELECT ID, LABELNR, PASS, ERRORCODE, VERIFIED FROM TAGSREAD
      WHERE PASS='PASS' AND WONR = 
      """ + question_id)

  rows = cursor.fetchall()
  if (len(rows) < 1):
    return HttpResponse("You're looking at question %s." % question_id)

  rowarray_list = []
  for row in rows:
    t = (row.ID, row.LABELNR.strip(), row.VERIFIED)
    rowarray_list.append(t)

  j = json.dumps(rowarray_list)
  return HttpResponse(j, content_type="application/json")
#return HttpResponse(j)

def results(request, question_id):
  response = "You're looking at the results of question %s."
  return HttpResponse(response % question_id)

def vote(request, question_id):
  return HttpResponse("You're voting on question %s." % question_id)

