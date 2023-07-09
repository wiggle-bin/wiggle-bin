from setuptools import setup, find_packages

VERSION = '0.0.10'
DESCRIPTION = 'WiggleR - Desktop research unit for making controlled experiments'
LONG_DESCRIPTION = 'WiggleR is a desktop research unit for making controlled experiments with worms. WiggleR can also be used inside the WiggleBin'

setup(
    name="wigglerapi",
    version=VERSION,
    author="Vincent Kranendonk",
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    packages=find_packages(),
    entry_points={
        'console_scripts': [
            'wigglerapi = wigglerapi.cli:main'
        ]
    },
    install_requires=[
        'fastapi',
        'pydantic',
        'uvicorn'
    ],
    keywords=['python'],
    classifiers=[]
)
