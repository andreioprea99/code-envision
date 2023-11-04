FROM python:3.6
WORKDIR /src
COPY requirements.txt /src
RUN pip install -r requirements.txt
COPY main.py /src
CMD [ "python", "-u", "main.py" ]