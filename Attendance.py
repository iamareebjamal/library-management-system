import xlrd
import xlwt
from bs4 import BeautifulSoup
import os
import urllib

import requests

url = "http://ctengg.amu.ac.in/student_attendance.php"


# ======================================================================================================================
#                               CLASS ATTENDANCE DEFINITION
class Attendance:
    facultyNum = str()
    percentage = dict()
    data = dict()

    def __init__(self, fno):
        self.facultyNum = fno
        self.subjects = list()
        self.data = {'faculty_number': self.facultyNum, 'submit': 'submit'}

    def loadAttendance(self):

        attendancePage = requests.post(url, self.data).text
        attendance = BeautifulSoup(attendancePage).find_all('tr')

        for at in attendance:
            [s, t, a, p, r, d] = at.find_all('td')
            subject = s.getText()
            self.subjects.append(subject)
            self.percentage.update({subject: p.getText()})

    def getSubjects(self):
        return self.subjects

    def getPecentage(self, s=None):
        if s is not None:
            try:
                return self.percentage[s]
            except KeyError:
                pass
        else:
            return self.percentage

    '''def addPecentage(self, p):
        for percent in p:
            self.percentage.append(percent)'''

    '''def addTotalClasses(self, t):
        for total in t:
            self.totalClasses.append(total)'''

    def __del__(self):

        return


# ======================================================================================================================
# ======================================================================================================================
def get_fac_postion(sheet):
    try:
        for r in range(0, sheet.nrows):
            for c in range(0, sheet.ncols):
                if sheet.cell_value(r, c) == 'Fac.No.':
                    return (r, c)
    except IndexError:
        pass


def get_fac_list(sheet):
    (row, col) = get_fac_postion(sheet)
    fac_list = list()
    for v in sheet.col_values(col, row + 1):
        fac_list.append(v)
    return fac_list


try:
    xl_workbook = xlrd.open_workbook('CO208.xlsx', 'html.parser')
    # print(xl_workbook)
    values = []
    sheet = xl_workbook.sheet_by_index(0)
    fac_list = get_fac_list(sheet)
    # print(list)
    try:
        for facNo in fac_list:

            student = Attendance(facNo)
            student.loadAttendance()
            print("\n\n" + facNo)
            for sub in student.getSubjects():
                print(sub + "\t", end=" ")
            print("\n")
            for sub in student.getSubjects():
                print(student.getPecentage(sub) + "\t", end=" ")
            del student
    except Exception as e:
        print("An exception occured : " + str(e))


except Exception:
    print(Exception)
