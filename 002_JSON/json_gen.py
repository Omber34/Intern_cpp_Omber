import json
import random 


if __name__ == "__main__":
    data = list( random.randint(-20,20) for i in range(20))
    with open("test_json.json","w") as fo:
        json.dump(data,fo)
