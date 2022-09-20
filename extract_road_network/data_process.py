import pandas as pd

df = pd.read_csv("Datasets_Yun/New_York_original.csv", header=0, sep=',')
print(df.head())

df2 = df.loc[:, ['LATITUDE','LONGITUDE']]
print(df2)

df3 = df2.dropna(axis=0, how='any')
print(df3)


# The latitude of New York City, NY, USA is 40.730610, and the longitude is -73.935242
df4 = df3[ (df3['LATITUDE']>38.730610) & (df3['LATITUDE']<42.730610) & (df3['LONGITUDE']>-75.935242) & (df3['LONGITUDE']<-71.935242) ]


# The latitude of Chicago, IL, USA is 41.881832, and the longitude is -87.623177
# df4 = df3[ (df3['Latitude']>39.881832) & (df3['Latitude']<43.881832) & (df3['Longitude']>-89.623177) & (df3['Longitude']<-85.623177) ]

# The latitude of Detroit, Michigan, the US is 42.331429, and the longitude is -83.045753.
# df4 = df3[ (df3['latitude']>40.331429) & (df3['latitude']<44.331429) & (df3['longitude']>-85.045753) & (df3['longitude']<-81.045753) ]

# The latitude of London, the UK is 51.509865, and the longitude is -0.118092.
# df4 = df3[ (df3['latitude']>49.509865) & (df3['latitude']<53.509865) & (df3['longitude']>-2.118092) & (df3['longitude']<2.118092) ]

# The latitude of San Francisco, CA, USA is 37.773972, and the longitude is -122.431297
# df4 = df3[ (df3['Latitude']>35.773972) & (df3['Latitude']<39.773972) & (df3['Longitude']>-124.431297) & (df3['Longitude']<-120.431297) ]
# print(df4)

df5 = df4.loc[:,['LONGITUDE', 'LATITUDE']] # make sure the order
print(df5)


df5.to_csv("Datasets_Yun/New_York_clean.csv", sep=' ', index=False, header=False)




