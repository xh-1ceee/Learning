import json
import math
import nltk
import re
import math
import numpy as np
import datetime
from nltk import word_tokenize, pos_tag
from nltk.corpus import wordnet as wn
from nltk.stem import WordNetLemmatizer
stopwords = ['i', 'me', 'my', 'myself', 'we', 'our', 'ours', 'ourselves', 'you', 'your', 'yours', 'yourself', 'yourselves',
             'he', 'him', 'his', 'himself', 'she', 'her', 'hers',
             'herself', 'it', 'its', 'itself', 'they', 'them', 'their', 'theirs', 'themselves', 'what', 'which', 'who', 'whom',
             'this', 'that', 'these', 'those', 'am', 'is', 'are',
             'was', 'were', 'be', 'been', 'being', 'have', 'has', 'had', 'having', 'do', 'does', 'did', 'doing', 'a', 'an',
             'the', 'and', 'but', 'if', 'or', 'because', 'as', 'until',
             'while', 'of', 'at', 'by', 'for', 'with', 'about', 'against', 'between', 'into', 'through', 'during', 'before',
             'after', 'above', 'below', 'to', 'from', 'up', 'down',
             'in', 'out', 'on', 'off', 'over', 'under', 'again', 'further', 'then', 'once', 'here', 'there', 'when', 'where',
             'why', 'how', 'all', 'any', 'both', 'each', 'few', 'more',
             'most', 'other', 'some', 'such', 'no', 'nor', 'not', 'only', 'own', 'same', 'so', 'than', 'too', 'very', 's', 't',
             'can', 'will', 'just', 'don', 'should', 'now', 'd', 'll',
             'm', 'o', 're', 've', 'y','!', ',' ,'.' ,'?' ,'-s' ,'-ly' ,'</s> ', 's','(',')', 'ain', 'aren', 'couldn', 'didn',
             'doesn', 'hadn', 'hasn', 'haven', 'isn', 'ma', 'mightn',
             'mustn', 'needn', 'shan', 'shouldn', 'wasn', 'weren', 'won', 'wouldn',
             'as','in',';','_','=',' ',':','$','&','|','￥','-']

class BM25(object):
    def __init__(self, docs):
        self.D = len(docs)
        self.avgdl = sum([len(doc)+0.0 for doc in docs]) / self.D
        self.docs = docs
        self.f = []     # 列表的每一个元素是一个dict，dict存储着一个文档中每个词的出现次数
        self.df = {}    # 存储每个词及出现了该词的文档数量
        self.idf = {}   # 存储每个词的idf值
        self.k1 = 1.2
        self.b = 0.75
        self.init()

    def init(self):
        for doc in self.docs:
            tmp = {}
            for word in doc:
                if word not in stopwords:
                    tmp[word] = tmp.get(word, 0) + 1  # 存储每个文档中每个词的出现次数
            self.f.append(tmp)
            for k in tmp.keys():
                self.df[k] = self.df.get(k, 0) + 1
        for k, v in self.df.items():
            self.idf[k] = math.log(self.D-v+0.5)-math.log(v+0.5)
        json_f = {"idf": self.idf,
                  "f": self.f}
        json_f = json.dumps(json_f)
        with open("model.json", 'w') as json_file:
            json_file.write(json_f)

    def sim(self, query, index):
        score = 0
        for word in query:
            if word not in self.f[index] or word in stopwords:
                continue
            d = len(self.docs[index])
            score += (self.idf[word]*self.f[index][word]*(self.k1+1)
                      / (self.f[index][word]+self.k1*(1-self.b+self.b*d
                                                      / self.avgdl)))
        return score

    def simall(self, query):
        scores = []
        for index in range(self.D):
            score = self.sim(query, index)
            scores.append(-score)
        scores = np.argsort(scores)[:10]
        return scores


def NDCG(logits, target, k):
    """
    Compute normalized discounted cumulative gain.
    :param logits: 2d array [batch_size x rel_docs_per_query]
    :param target: 2d array [batch_size x rel_docs_per_query]
    :return: mean average precision [a float value]
    """
    #assert logits.shape == target.shape
    assert logits.shape[1] >= k, 'NDCG@K cannot be computed, invalid value of K.'

    #indices = np.argsort(-logits, 1)
    indices = logits
    NDCG = 0
    for i in range(indices.shape[0]):
        DCG_ref = 0
        num_rel_docs = np.count_nonzero(target[i])
        for j in range(indices.shape[1]):
            if j == k:
                break
            if target[i, indices[i, j]] == 1:
                DCG_ref += 1 / np.log2(j + 2)
        DCG_gt = 0
        for j in range(num_rel_docs):
            if j == k:
                break
            DCG_gt += 1 / np.log2(j + 2)
        NDCG += DCG_ref / DCG_gt

    return NDCG / indices.shape[0]

def MRR(logits, target, k):
    """
    Compute mean reciprocal rank.
    :param logits: 2d array [batch_size x rel_docs_per_query]
    :param target: 2d array [batch_size x rel_docs_per_query]
    :return: mean reciprocal rank [a float value]
    """
    #assert logits.shape == target.shape
    assert logits.shape[1] == k
    indices_k = logits
    # num_doc = logits.shape[1]
    #indices_k = np.argsort(-logits, 1)[:, :k]  # 取topK 的index   [n, k]

    reciprocal_rank = 0
    for i in range(indices_k.shape[0]):
        for j in range(indices_k.shape[1]):
            if target[i, indices_k[i, j]] == 1:
                reciprocal_rank += 1.0 / (j + 1)
                break

    return reciprocal_rank / indices_k.shape[0]

def wn_tags(pos_tag):
    if pos_tag.startswith('J'): return wn.ADJ
    elif pos_tag.startswith('V'): return wn.VERB
    elif pos_tag.startswith('N'): return wn.NOUN
    elif pos_tag.startswith('R'): return wn.ADV
    else: return None

def clean_word(word):
    comp = re.compile('[^A-Z^a-z^ ]')
    tmp = comp.sub('', word)
    if tmp == "":
        return tmp
    else:
        return word

def get_data():
    f_d = json.load(open("documents.json", 'r', encoding="utf-8", errors="ignore"))
    f = json.load(open("validationset.json", 'r'))
    print("valid set")
    #f = json.load(open("testset_no_label.json", 'r'))
    #print("test set")

    docs, queries, labels = [], [], []
    #docs,queries = [],[]
    docs_id = {}
    num = 0
    wnl = WordNetLemmatizer()
    for key in f_d:
        tokens = nltk.word_tokenize(f_d[key])
        tokens = [w.lower() for w in tokens]
        tokens = [w for w in tokens if w not in stopwords]
        tokens = [clean_word(word) for word in tokens if clean_word(word) != '']
        tagged_tokens = pos_tag(tokens)
        tmp = []
        for tag in tagged_tokens:
            wordnet_pos = wn_tags(tag[1]) or wn.NOUN
            tmp.append(wnl.lemmatize(tag[0], pos=wordnet_pos))  #词形归并
        docs.append(tmp)
        docs_id[key] = num
        num += 1
    num_doc = num
    print(len(docs))
    print("docs[0]:", docs[0])
    print("num_doc:", num_doc)
    tmpFlag = 0
    for key in f["queries"].keys():
        tmp = nltk.word_tokenize(f["queries"][key])
        tmp = [word.lower() for word in tmp]
        tmp = [word for word in tmp if word not in stopwords]
        tmp = [clean_word(word) for word in tmp if clean_word(word) != '']
        tagged_tokens = pos_tag(tmp)
        tokens = []
        for tag in tagged_tokens:
            wordnet_pos = wn_tags(tag [1]) or wn.NOUN
            tokens.append(wnl.lemmatize(tag[0], pos=wordnet_pos))  #词形归并
        queries.append(tokens)

        label = f["labels"][key]
        label_ = np.zeros(num_doc)
        for i in range(len(label)):
            # label[i] = docs_id[str(label[i])]
            label_[docs_id[str(label[i])]] = 1
        # one_hot = np.eye(num_doc)[label]  # [x, num_doc]
        # label = np.sum(one_hot, axis=0)  # [num_doc]
        labels.append(label_)
        #  tmpFlag += 1
        #if tmpFlag == 10: break
    labels = np.array(labels)

    print("num_query:", len(queries))
    #return docs, queries, labels    # 此处返回的是分词后结果
    return docs, queries,labels


if __name__ == '__main__':
    tic = datetime.datetime.now() 
    docs, queries, labels = get_data()
    #docs, queries = get_data()
    toc = datetime.datetime.now()
    print("data preprocess finished in {}".format(toc - tic))

    tic = datetime.datetime.now()
    s = BM25(docs)
    toc = datetime.datetime.now()
    print("BM25 Model finished in {}".format(toc - tic))

    tic = datetime.datetime.now()
    scores = []
    labels = labels
    toc = datetime.datetime.now()
    for i in range(len(queries)):
        score = s.simall(queries[i])
        scores.append(score)
        if len(scores) % 100 == 0:
            print(len(scores))

    logits = np.array(scores)
    
    print(logits.shape)
    #np.save("2017.npy", logits)    # 最终提交文件2017xxx.npy（学号命名）
    toc = datetime.datetime.now()
    print("test finished in {}".format(toc - tic))

    toc = datetime.datetime.now()
    print("logits finished in {}".format(toc - tic))

    tic = datetime.datetime.now()
    ndcg_10 = NDCG(logits, labels, 10)
    print('NDCG@10 - ', ndcg_10)

    mrr = MRR(logits, labels, 10)
    print('MRR@10 - ', mrr)

    #precision, recall, F1_Score = Precision_Recall_F1Score(logits, labels, 10)
    #print("precision@10 -  ", precision, "\trecall@10 - ", recall, "\tF1_Score@10 - ", F1_Score)
    toc = datetime.datetime.now()
    print("test finished in {}".format(toc - tic))

