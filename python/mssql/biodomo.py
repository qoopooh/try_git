#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0.0'

#HOST='aaebio\\bsserver'
HOST='192.168.1.15'
USER='sa'
PASSWORD='sa'
DATABASE='domotics'
OUTPUT_JS='output.js'

###############################################################################
import sys, json
import web, pymssql

COMMAND = """
SELECT *
FROM command
"""

def ask(query, as_dict=False):
    conn = pymssql.connect(host=HOST, user=USER, password=PASSWORD,
        database=DATABASE, charset='utf8', as_dict=as_dict)

    cur = conn.cursor()
    cur.execute(query)
    rows = cur.fetchall()
    conn.close()
    return rows
    

class index():
    def GET(self):
        return render.idx()

class Table():

    def GET(self):
        q = COMMAND
        return self.show_resp(q)

    def get_query(self, i):
        self._input = i
        act = i['action']
        if act=='WIT_NT': q=WIT_NT
        elif act=='WIT_NT_ID': q=WIT_NT_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='WIT_CUS': q=WIT_CUS
        elif act=='WIT_CUS_ID':
            if i['cid'] is not None:
                q=WIT_CUS_ID_CID.format(tid=i['tid'],cid=i['cid'])
            else:
                q=WIT_CUS_ID.format(tid=i['tid'])
        elif act=='WIT_STO': q=WIT_STO
        elif act=='WIT_STO_ID': q=WIT_STO_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='WIT_REJ': q=WIT_REJ
        elif act=='WIT_REJ_ID': q=WIT_REJ_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='INV_NT': q=INV_NT
        elif act=='INV_NT_ID': q=INV_NT_ID.format(sz=i['sz'])
        elif act=='INV_CUS': q=INV_CUS
        elif act=='INV_CUS_ID': q=INV_CUS_ID.format(sz=i['sz'])
        elif act=='INV_STO': q=INV_STO
        elif act=='INV_STO_ID': q=INV_STO_ID.format(sz=i['sz'])
        elif act=='INV_REJ': q=INV_REJ
        elif act=='INV_REJ_ID': q=INV_REJ_ID.format(sz=i['sz'])
        else: q=IDENTIFY.format(tid=i['sn'])
        return q

    def show_resp(self, query):
        resp = ask(query, as_dict=True)
        for row in resp:
            for k in row.keys():
                if isinstance(k, int):
                    del row[k]

        return render.tyretable(resp)

    def filter_PRI(self, resp):
        if self._input['action'] != 'WIT_STO' or len(resp) < 1:
            return resp
        if isinstance(resp[0], dict):
            for row in resp:
                if row['ProdTransDetail_ProdTrans_ID'][:3] == 'PRI':
                    row['Comp_Name'] = None
                    row['Comp_ID'] = 0
        else:
            l, i, tmp, resp = len(resp), 0, resp, []
            while i<l:
                if tmp[i][0][:3] == 'PRI':
                    resp.append((tmp[i][0], None, tmp[i][2], 0))
                else:
                    resp.append(tmp[i])
                i += 1

        return resp

class Json(Table):

    def GET(self):
        q = COMMAND
        return self.show_resp(q)

    def show_resp(self, query):
        resp = ask(query)
        web.header('Content-Type', 'application/json')
        j = json.dumps(resp, ensure_ascii=False, indent=2).encode('utf8')
        print 'show_resp1', resp, j
        with open(OUTPUT_JS, 'w') as f:
            print >> f, resp
        return j

urls = (
    '/', 'index',
    '/table/', 'Table',
    '/json/', 'Json',
)
template_globals = {
    'app_path': lambda p: web.ctx.homepath + p,
}
render = web.template.render('templates/', globals=template_globals)

if __name__ == '__main__':
    app = web.application(urls, globals())
    app.run()

