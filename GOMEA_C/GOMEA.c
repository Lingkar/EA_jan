/*-=-=-=-=-=-=-=-=-=-=-=-=-=-= Section Header -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * GOMEA.c
 *
 * Copyright (c) Peter A.N. Bosman
 *
 * The software in this file is the proprietary information of
 * Peter A.N. Bosman.
 *
 * IN NO EVENT WILL THE AUTHOR OF THIS SOFTWARE BE LIABLE TO YOU FOR ANY
 * DAMAGES, INCLUDING BUT NOT LIMITED TO LOST PROFITS, LOST SAVINGS, OR OTHER
 * INCIDENTIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR THE INABILITY
 * TO USE SUCH PROGRAM, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGES, OR FOR ANY CLAIM BY ANY OTHER PARTY. THE AUTHOR MAKES NO
 * REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE SOFTWARE, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT. THE
 * AUTHOR SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY ANYONE AS A RESULT OF
 * USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
 *
 * Gene-pool Optimal Mixing Evolutionary Algorithm
 *
 * In this implementation, maximization is assumed.
 *
 * The software in this file is the result of (ongoing) scientific research.
 * The following people have been actively involved in this research over
 * the years:
 * - Peter A.N. Bosman
 * - Dirk Thierens
 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-=-= Section Includes -=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-= Section Header Functions -=-=-=-=-=-=-=-=-=-=-=-=*/
void *Malloc( long size );
void interpretCommandLine( int argc, char **argv );
void parseCommandLine( int argc, char **argv );
void parseOptions( int argc, char **argv, int *index );
void printAllInstalledProblems( void );
void optionError( char **argv, int index );
void parseParameters( int argc, char **argv, int *index );
void printUsage( void );
void checkOptions( void );
void printVerboseOverview( void );
double randomRealUniform01( void );
int randomInt( int maximum );
int *randomPermutation( int n );
char *installedProblemName( int index );
int numberOfInstalledProblems( void );
void installedProblemEvaluation( int index, char *parameters, double *objective_value, double *constraint_value );
void onemaxFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value );
void deceptiveTrap4TightEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value );
void deceptiveTrap4LooseEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value );
void deceptiveTrap5TightEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value );
void deceptiveTrap5LooseEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value );
void deceptiveTrapKTightEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value, int k );
void deceptiveTrapKLooseEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value, int k );
void maxcutProblemInitialization();
short maxcutReadInstanceFromFile();
void maxcutProblemNoitazilaitini();
void maxcutProblemEvaluation( char *parameters, double *objective_value, double *constraint_value );
void maxcutPartialProblemInitialization();
void maxcutBuildAdjacencyList();
void maxcutPartialProblemNoitazilaitini();
void maxcutPartialProblemEvaluation( char *parameters, double *objective_value, double *constraint_value, int number_of_touched_parameters, int *touched_parameters_indices, char *parameters_before, double objective_value_before, double constraint_value_before );
void initialize();
void initializeMemory();
void initializeRandomNumberGenerator();
void initializeProblem( int index );
void initializeValueToReach();
void initializePopulationAndFitnessValues();
void writeGenerationalStatistics();
void writeGenerationalSolutions( char is_final_generation );
void writeGenerationalSolutionsBest( char is_final_generation );
void writeGenerationalSolutionsBestEver();
void writeRunningTime( char *filename );
char checkTerminationCondition();
char checkNumberOfEvaluationsTerminationCondition();
char checkVTRTerminationCondition();
void determineBestSolutionInCurrentPopulation( int *index_of_best );
char checkFitnessVarianceTermination();
void selectFinalSurvivors();
void updateBestPrevGenSolution();
char betterFitness( double objective_value_x, double constraint_value_x, double objective_value_y, double constraint_value_y );
char equalFitness( double objective_value_x, double constraint_value_x, double objective_value_y, double constraint_value_y );
void makeOffspring();
void initializeUnivariateFOS();
void learnLinkageTree();
double *estimateParametersForSingleBinaryMarginal( int *indices, int number_of_indices, int *factor_size );
int determineNearestNeighbour( int index, double **S_matrix, int mpm_length );
void printLinkageModel();
double log2( double x );
void generateAndEvaluateNewSolutionsToFillOffspring();
char *generateNewSolution( int which, double *obj, double *con );
void ezilaitiniProblem( int problem_index );
void ezilaitiniMemory( void );
void run();
long getMilliSecondsRunning();
int main( int argc, char **argv );
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=- Section Global Variables -=-=-=-=-=-=-=-=-=-=-=-=-*/
char    **population,                           /* Population solutions. */
        **offspring,                            /* Offspring solutions. */
         *best_prevgen_solution,                /* The best solution found in all previous generations. */
         *best_ever_evaluated_solution,         /* The best ever evaluated solution. */
         file_id;                              /* The id of the files used */
short     write_generational_statistics,        /* Whether to compute and write statistics every generation (0 = no). */
          write_generational_solutions,         /* Whether to write the population every generation (0 = no). */
          print_verbose_overview,               /* Whether to print a overview of settings (0 = no). */
          use_vtr,                              /* Whether to terminate at the value-to-reach (VTR) (0 = no). */
          vtr_hit_has_happened,                 /* Whether the vtr has been hit yet. */
          print_linkage_model_contents,         /* Whether to print the linkage_model structure every generation. */
          use_univariate_fos;                   /* Whether to use a univariate factorization as the linkage model. */
int       problem_index,                        /* The index of the optimization problem. */
          number_of_parameters,                 /* The number of parameters to be optimized. */
          number_of_generations,                /* The current generation count. */
          population_size,                      /* The size of the population. */
          offspring_size,                       /* The size of the offspring. */
          maximum_number_of_evaluations,        /* The maximum number of evaluations. */
          no_improvement_stretch,               /* The number of subsequent generations without an improvement. */
        **mpm,                                  /* The marginal product model (mpm). */
         *mpm_number_of_indices,                /* The number of variables in each factor in the mpm. */
          mpm_length,                           /* The number of factors in the mpm. */
        **linkage_model,                                   /* The linkage model. */
         *linkage_model_number_of_indices,                 /* The number of variables in each factor in the lt. */
          linkage_model_length;                            /* The number of factors in the lt. */
long      number_of_evaluations,                /* The current number of times a function evaluation was performed. */
          timestamp_start;                      /* The time stamp in milliseconds for when the algorithm was started. */
double   *objective_values,                     /* Objective values for population members. */
         *constraint_values,                    /* Sum of all constraint violations for population members. */
         *objective_values_offspring,           /* Objective values of offspring solutions. */
         *constraint_values_offspring,          /* Sum of all constraint violations of offspring solutions. */
          best_prevgen_objective_value,         /* Objective value of best solution in all previous generations. */
          best_prevgen_constraint_value,        /* Constraint value of best solution in all previous generations. */
          fitness_variance_tolerance,           /* The minimum fitness variance level that is allowed. */
          vtr,                                  /* The value-to-reach (function value of best solution that is feasible). */
        **MI_matrix,                            /* Mutual information between any two variables. */
          best_ever_evaluated_objective_value,  /* The best ever evaluated objective value. */
          best_ever_evaluated_constraint_value; /* The best ever evaluated constraint value. */
int64_t   random_seed,                          /* The seed used for the random-number generator. */
          random_seed_changing;                 /* Internally used variable for randomly setting a random seed. */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-=-= Section Constants -=-=-=-=-=-=-=-=-=-=-=-=-=-*/
#define FALSE 0
#define TRUE 1
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-= Section Elementary Operations -=-=-=-=-=-=-=-=-=-=-*/
/**
 * Allocates memory and exits the program in case of a memory allocation failure.
 */
void *Malloc( long size )
{
  void *result;

  result = (void *) malloc( size );
  if( !result )
  {
    printf("\n");
    printf("Error while allocating memory in Malloc( %ld ), aborting program.", size);
    printf("\n");

    exit( 0 );
  }

  return( result );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=- Section Interpret Command Line -=-=-=-=-=-=-=-=-=-=-*/
/**
 * Parses and checks the command line.
 */
void interpretCommandLine( int argc, char **argv )
{
  parseCommandLine( argc, argv );
  
  checkOptions();
}

/**
 * Parses the command line.
 * For options, see printUsage.
 */
void parseCommandLine( int argc, char **argv )
{
  int index;

  index = 1;

  parseOptions( argc, argv, &index );
  
  parseParameters( argc, argv, &index );
}

/**
 * Parses only the options from the command line.
 */
void parseOptions( int argc, char **argv, int *index )
{
  double dummy;

  write_generational_statistics = 0;
  write_generational_solutions  = 0;
  print_verbose_overview        = 0;
  print_linkage_model_contents  = 0;
  use_vtr                       = 0;
  use_univariate_fos            = 0;

  for( ; (*index) < argc; (*index)++ )
  {
    if( argv[*index][0] == '-' )
    {
      /* If it is a negative number, the option part is over */
      if( sscanf( argv[*index], "%lf", &dummy ) && argv[*index][1] != '\0' )
        break;

      if( argv[*index][1] == '\0' )
        optionError( argv, *index );
      else if( argv[*index][2] != '\0' )
        optionError( argv, *index );
      else
      {
        switch( argv[*index][1] )
        {
          case '?': printUsage(); break;
          case 'P': printAllInstalledProblems(); break;
          case 's': write_generational_statistics = 1; break;
          case 'w': write_generational_solutions  = 1; break;
          case 'v': print_verbose_overview        = 1; break;
          case 'l': print_linkage_model_contents  = 1; break;
          case 'U': use_univariate_fos            = 1; break;
          default : optionError( argv, *index );
        }
      }
    }
    else /* Argument is not an option, so option part is over */
     break;
  }
}

/**
 * Writes the names of all installed problems to the standard output.
 */
void printAllInstalledProblems( void )
{
  int i, n;

  n = numberOfInstalledProblems();
  printf("Installed optimization problems:\n");
  for( i = 0; i < n; i++ )
    printf("%3d: %s\n", i, installedProblemName( i ));

  exit( 0 );
}

/**
 * Informs the user of an illegal option and exits the program.
 */
void optionError( char **argv, int index )
{
  printf("Illegal option: %s\n\n", argv[index]);

  printUsage();
}

/**
 * Parses only the parameters from the command line.
 */
void parseParameters( int argc, char **argv, int *index )
{
  int noError;

  if( (argc - *index) != 6 )
  {
    printf("Number of parameters is incorrect, require 6 parameters (you provided %d).\n\n", (argc - *index));

    printUsage();
  }

  noError = 1;
  noError = noError && sscanf( argv[*index+0], "%d", &problem_index );
  noError = noError && sscanf( argv[*index+1], "%d", &number_of_parameters );
  noError = noError && sscanf( argv[*index+2], "%d", &population_size );
  noError = noError && sscanf( argv[*index+3], "%d", &maximum_number_of_evaluations );
  noError = noError && sscanf( argv[*index+4], "%lf", &fitness_variance_tolerance );
  noError = noError && sscanf(argv[*index+5], "%c", &file_id);

  if( !noError )
  {
    printf("Error parsing parameters.\n\n");

    printUsage();
  }
}

/**
 * Prints usage information and exits the program.
 */
void printUsage( void )
{
  printf("Usage: GOMEA [-?] [-P] [-s] [-w] [-v] [-l] [-r] pro dim pop eva vtr tol unique_id\n");
  printf(" -?: Prints out this usage information.\n");
  printf(" -P: Prints out a list of all installed optimization problems.\n");
  printf(" -s: Enables computing and writing of statistics every generation.\n");
  printf(" -w: Enables writing of solutions and their fitnesses every generation.\n");
  printf(" -v: Enables verbose mode. Prints the settings before starting the run.\n");
  printf(" -l: Enables printing the contents of the linkage model every generation.\n");
  printf(" -U: Enables the use of a univariate FOS instead of a linkage tree.\n");
  printf("\n");
  printf("  pro: Index of optimization problem to be solved (maximization).\n");
  printf("  dim: Number of parameters.\n");
  printf("  pop: Population size.\n");
  printf("  eva: Maximum number of evaluations allowed.\n");
  printf("  vtr: The value to reach. If the objective value of the best feasible solution reaches\n");
  printf("       this value, termination is enforced (if -r is specified).\n");
  printf("  tol: The tolerance level for fitness variance (i.e. minimum fitness variance)\n");
  printf("  unique_id: The unique id used for the specific files on the fs, to enable parallelism (e.g. 5)\n");
  exit( 0 );
}

/**
 * Checks whether the selected options are feasible.
 */
void checkOptions( void )
{
  if( number_of_parameters < 1 )
  {
    printf("\n");
    printf("Error: number of parameters < 1 (read: %d). Require number of parameters >= 1.", number_of_parameters);
    printf("\n\n");

    exit( 0 );
  }

  if( population_size < 1 )
  {
    printf("\n");
    printf("Error: population size < 1 (read: %d). Require population size >= 1.", population_size);
    printf("\n\n");

    exit( 0 );
  }

  if( installedProblemName( problem_index ) == NULL )
  {
    printf("\n");
    printf("Error: unknown index for problem (read index %d).", problem_index );
    printf("\n\n");

    exit( 0 );
  }
}

/**
 * Prints the settings as read from the command line.
 */
void printVerboseOverview( void )
{
  printf("### Settings ######################################\n");
  printf("#\n");
  printf("# Statistics writing every generation: %s\n", write_generational_statistics ? "enabled" : "disabled");
  printf("# Population file writing            : %s\n", write_generational_solutions ? "enabled" : "disabled");
  printf("# Use of value-to-reach (vtr)        : %s\n", use_vtr ? "enabled" : "disabled");
  printf("#\n");
  printf("###################################################\n");
  printf("#\n");
  printf("# Problem                 = %s\n", installedProblemName( problem_index ));
  printf("# Number of parameters    = %d\n", number_of_parameters);
  printf("# Population size         = %d\n", population_size);
  printf("# Maximum numb. of eval.  = %d\n", maximum_number_of_evaluations);
  printf("# Value to reach (vtr)    = %e\n", vtr);
  printf("# Fitness var. tolerance  = %e\n", fitness_variance_tolerance);
  printf("# Random seed             = %ld\n", (long) random_seed);
  printf("#\n");
  printf("###################################################\n");
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-= Section Random Numbers -=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * Returns a random double, distributed uniformly between 0 and 1.
 */
double randomRealUniform01( void )
{
  int64_t n26, n27;
  double  result;

  random_seed_changing = (random_seed_changing * 0x5DEECE66DLLU + 0xBLLU) & ((1LLU << 48) - 1);
  n26                  = (int64_t)(random_seed_changing >> (48 - 26));
  random_seed_changing = (random_seed_changing * 0x5DEECE66DLLU + 0xBLLU) & ((1LLU << 48) - 1);
  n27                  = (int64_t)(random_seed_changing >> (48 - 27));
  result               = (((int64_t)n26 << 27) + n27) / ((double) (1LLU << 53));

  return( result );
}
        
/**
 * Returns a random integer, distributed uniformly between 0 and maximum.
 */
int randomInt( int maximum )
{
  int result;
  
  result = (int) (((double) maximum)*randomRealUniform01());
  
  return( result );
}

/**
 * Returns a random compact (using integers 0,1,...,n-1) permutation
 * of length n using the Fisher-Yates shuffle.
 */
int *randomPermutation( int n )
{
  int i, j, dummy, *result;

  result = (int *) Malloc( n*sizeof( int ) );
  for( i = 0; i < n; i++ )
    result[i] = i;

  for( i = n-1; i > 0; i-- )
  {
    j         = randomInt( i+1 );
    dummy     = result[j];
    result[j] = result[i];
    result[i] = dummy;
  }

  return( result );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-=-= Section Problems -=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * Returns the name of an installed problem.
 */
char *installedProblemName( int index )
{
  switch( index )
  {
    case  0: return( (char *) "Onemax" );
    case  1: return( (char *) "Deceptive Trap 4 - Tight Encoding" );
    case  2: return( (char *) "Deceptive Trap 4 - Loose Encoding" );
    case  3: return( (char *) "Deceptive Trap 5 - Tight Encoding" );
    case  4: return( (char *) "Deceptive Trap 5 - Loose Encoding" );
    case  5: return( (char *) "MAXCUT (reads from file maxcut_instance_{unique_id}.txt)" );
  }

  return( NULL );
}

/**
 * Returns the number of problems installed.
 */
int numberOfInstalledProblems( void )
{
  static int result = -1;
  
  if( result == -1 )
  {
    result = 0;
    while( installedProblemName( result ) != NULL )
      result++;
  }
  
  return( result );
}

/**
 * Computes the value of the single objective and the sum of all
 * constraint violations.
 */
void installedProblemEvaluation( int index, char *parameters, double *objective_value, double *constraint_value )
{
  int i;
  char filename_1[1000], filename_2[1000];

  number_of_evaluations++;

  *objective_value  = 0.0;
  *constraint_value = 0.0;

  switch( index )
  {
    case  0: onemaxFunctionProblemEvaluation( parameters, objective_value, constraint_value ); break;
    case  1: deceptiveTrap4TightEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value ); break;
    case  2: deceptiveTrap4LooseEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value ); break;
    case  3: deceptiveTrap5TightEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value ); break;
    case  4: deceptiveTrap5LooseEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value ); break;
    case  5: maxcutProblemEvaluation( parameters, objective_value, constraint_value ); break;
  }

  if( vtr_hit_has_happened == 0 )
  {
    if( (*constraint_value) == 0 && (*objective_value) >= vtr  )
    {
      vtr_hit_has_happened = 1;
        sprintf(filename_1, "vtr_hitting_time_%s.dat", &file_id);

        writeRunningTime( filename_1 );
    }
  }

  if( (number_of_evaluations == 1) || betterFitness( *objective_value, *constraint_value, best_ever_evaluated_objective_value, best_ever_evaluated_constraint_value ) )
  {
    for( i = 0; i < number_of_parameters; i++ )
      best_ever_evaluated_solution[i] = parameters[i];
    best_ever_evaluated_objective_value  = *objective_value;
    best_ever_evaluated_constraint_value = *constraint_value;


      sprintf(filename_2, "best_ever_hitting_time_%s.dat", &file_id);



      writeRunningTime( (char *) filename_2 );
  }
}

void onemaxFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value )
{
  int    i;
  double result;

  result = 0.0;
  for( i = 0; i < number_of_parameters; i++ )
    result += (parameters[i] == TRUE) ? 1 : 0;

  *objective_value  = result;
  *constraint_value = 0;
}

void deceptiveTrap4TightEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value )
{
  deceptiveTrapKTightEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value, 4 );
}

void deceptiveTrap4LooseEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value )
{
  deceptiveTrapKLooseEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value, 4 );
}

void deceptiveTrap5TightEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value )
{
  deceptiveTrapKTightEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value, 5 );
}

void deceptiveTrap5LooseEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value )
{
  deceptiveTrapKLooseEncodingFunctionProblemEvaluation( parameters, objective_value, constraint_value, 5 );
}

void deceptiveTrapKTightEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value, int k )
{
  int    i, j, m, u;
  double result;

  if( number_of_parameters % k != 0 )
  {
    printf("Error in evaluating deceptive trap k: Number of parameters is not a multiple of k.\n");
    exit( 0 );
  }

  m      = number_of_parameters / k;
  result = 0.0;
  for( i = 0; i < m; i++ )
  {
    u = 0;
    for( j = 0; j < k; j++ )
      u += (parameters[i*k+j] == TRUE) ? 1 : 0;

    if( u == k )
      result += 1.0;
    else
      result += ((double) (k-1-u))/((double) k);
  }

  *objective_value  = result;
  *constraint_value = 0;
}

void deceptiveTrapKLooseEncodingFunctionProblemEvaluation( char *parameters, double *objective_value, double *constraint_value, int k )
{
  int    i, j, m, u;
  double result;

  if( number_of_parameters % k != 0 )
  {
    printf("Error in evaluating deceptive trap k: Number of parameters is not a multiple of k.\n");
    exit( 0 );
  }

  m      = number_of_parameters / k;
  result = 0.0;
  for( i = 0; i < m; i++ )
  {
    u = 0;
    for( j = 0; j < k; j++ )
      u += (parameters[i+m*j] == TRUE) ? 1 : 0;

    if( u == k )
      result += 1.0;
    else
      result += ((double) (k-1-u))/((double) k);
  }

  *objective_value  = result;
  *constraint_value = 0;
}

int    **maxcut_edges, number_of_maxcut_edges;
double  *maxcut_edges_weights;
void maxcutProblemInitialization()
{
  if( !maxcutReadInstanceFromFile() )
  {
    printf("\n");
    printf("Error: couldn't find maxcut instance.");
    printf("\n\n");

    exit( 0 );
  }
}

short maxcutReadInstanceFromFile()
{
  char  c, string[1000], substring[1000], filename[1000];
  int   i, j, k, q, number_of_vertices;
  FILE *file;

    sprintf(filename, "maxcut_instance_%s.txt", &file_id);

  file = fopen( filename, "r" );
  if( file == NULL )
    return( 0 );

  c = fgetc( file );
  k = 0;
  while( c != '\n' && c != EOF )
  {
    string[k] = (char) c;
    c      = fgetc( file );
    k++;
  }
  string[k] = '\0';

  q = 0;
  j = 0;
  while( (string[j] != ' ') && (j < k) )
  {
    substring[q] = string[j];
    q++;
    j++;
  }
  substring[q] = '\0';
  j++;

  number_of_vertices = atoi( substring );
  if( number_of_vertices != number_of_parameters )
  {
    printf("Error during reading of maxcut instance (%s):\n",filename);
    printf("  Read number of vertices: %d\n", number_of_vertices);
    printf("  Doesn't match number of parameters on command line: %d\n", number_of_parameters);
    exit( 1 );
  }

  q = 0;
  while( (string[j] != ' ') && (j < k) )
  {
    substring[q] = string[j];
    q++;
    j++;
  }
  substring[q] = '\0';
  j++;

  number_of_maxcut_edges = atoi( substring );
  maxcut_edges = (int **) Malloc( number_of_maxcut_edges*sizeof( int * ) );
  for( i = 0; i < number_of_maxcut_edges; i++ )
    maxcut_edges[i] = (int *) Malloc( 2*sizeof( int ) );
  maxcut_edges_weights = (double *) Malloc( number_of_maxcut_edges*sizeof( double ) );
  
  i = 0;
  c = fgetc( file );
  k = 0;
  while( c != '\n' && c != EOF )
  {
    string[k] = (char) c;
    c      = fgetc( file );
    k++;
  }
  string[k] = '\0';
  while( k > 0 )
  {
    q = 0;
    j = 0;
    while( (string[j] != ' ') && (j < k) )
    {
      substring[q] = string[j];
      q++;
      j++;
    }
    substring[q] = '\0';
    j++;

    maxcut_edges[i][0] = atoi( substring )-1;

    q = 0;
    while( (string[j] != ' ') && (j < k) )
    {
      substring[q] = string[j];
      q++;
      j++;
    }
    substring[q] = '\0';
    j++;

    maxcut_edges[i][1] = atoi( substring )-1;

    q = 0;
    while( (string[j] != ' ') && (j < k) )
    {
      substring[q] = string[j];
      q++;
      j++;
    }
    substring[q] = '\0';
    j++;

    maxcut_edges_weights[i] = atof( substring );
    i++;

    c = fgetc( file );
    k = 0;
    while( c != '\n' && c != EOF )
    {
      string[k] = (char) c;
      c      = fgetc( file );
      k++;
    }
    string[k] = '\0';
  }
  
  fclose( file );

  return( 1 );
}

void maxcutProblemNoitazilaitini()
{
  int i;
  
  for( i = 0; i < number_of_maxcut_edges; i++ )
    free( maxcut_edges[i] );
  free( maxcut_edges );
  free( maxcut_edges_weights );
}

void maxcutProblemEvaluation( char *parameters, double *objective_value, double *constraint_value )
{
  int    i;
  double result;

  result = 0.0;
  for( i = 0; i < number_of_maxcut_edges; i++ )
  {
    if( parameters[maxcut_edges[i][0]] != parameters[maxcut_edges[i][1]] )
      result += maxcut_edges_weights[i];
  }

  *objective_value  = result;
  *constraint_value = 0;
}

int    **maxcut_neighbors, *maxcut_number_of_neighbors;
double **maxcut_neighbors_weights;
void maxcutPartialProblemInitialization()
{
  maxcutProblemInitialization();
  maxcutBuildAdjacencyList();
}

void maxcutBuildAdjacencyList()
{
  int i;

  /* First count the number of neighbors for each node */
  maxcut_number_of_neighbors = (int *) Malloc( number_of_parameters*sizeof( int ) );
  for( i = 0; i < number_of_parameters; i++ )
    maxcut_number_of_neighbors[i] = 0;

  for( i = 0; i < number_of_maxcut_edges; i++ )
  {
    maxcut_number_of_neighbors[maxcut_edges[i][0]]++;
    maxcut_number_of_neighbors[maxcut_edges[i][1]]++;
  }

  /* Then create the data structures */
  maxcut_neighbors         = (int **) Malloc( number_of_parameters*sizeof( int * ) );
  maxcut_neighbors_weights = (double **) Malloc( number_of_parameters*sizeof( double * ) );
  for( i = 0; i < number_of_parameters; i++ )
  {
    maxcut_neighbors[i] = (int *) Malloc( maxcut_number_of_neighbors[i]*sizeof( int ) );
    maxcut_neighbors_weights[i] = (double *) Malloc( maxcut_number_of_neighbors[i]*sizeof( double ) );
  }

  /* Finally, fill the data structures */
  for( i = 0; i < number_of_parameters; i++ )
    maxcut_number_of_neighbors[i] = 0;

  for( i = 0; i < number_of_maxcut_edges; i++ )
  {
    maxcut_neighbors[maxcut_edges[i][0]][maxcut_number_of_neighbors[maxcut_edges[i][0]]]         = maxcut_edges[i][1];
    maxcut_neighbors[maxcut_edges[i][1]][maxcut_number_of_neighbors[maxcut_edges[i][1]]]         = maxcut_edges[i][0];
    maxcut_neighbors_weights[maxcut_edges[i][0]][maxcut_number_of_neighbors[maxcut_edges[i][0]]] = maxcut_edges_weights[i];
    maxcut_neighbors_weights[maxcut_edges[i][1]][maxcut_number_of_neighbors[maxcut_edges[i][1]]] = maxcut_edges_weights[i];

    maxcut_number_of_neighbors[maxcut_edges[i][0]]++;
    maxcut_number_of_neighbors[maxcut_edges[i][1]]++;
  }
}

void maxcutPartialProblemNoitazilaitini()
{
  int i;

  for( i = 0; i < number_of_parameters; i++ )
  {
    free( maxcut_neighbors[i] );
    free( maxcut_neighbors_weights[i] );
  }
  free( maxcut_neighbors );
  free( maxcut_neighbors_weights );
  free( maxcut_number_of_neighbors );

  maxcutProblemNoitazilaitini();
}

void maxcutPartialProblemEvaluation( char *parameters, double *objective_value, double *constraint_value, int number_of_touched_parameters, int *touched_parameters_indices, char *parameters_before, double objective_value_before, double constraint_value_before )
{
  char   *parameters_after_touching;
  int     i, j;
  double  objective_value_new;

  if( touched_parameters_indices == NULL )
    maxcutProblemEvaluation( parameters, objective_value, constraint_value );
  else
  {
    /* Extract what the nodes should become and revert current solution to what it was before */
    parameters_after_touching = (char *) Malloc( number_of_touched_parameters*sizeof( char ) );
    for( i = 0; i < number_of_touched_parameters; i++ )
    {
      parameters_after_touching[i]              = parameters[touched_parameters_indices[i]];
      parameters[touched_parameters_indices[i]] = parameters_before[touched_parameters_indices[i]];
    }

    /* Incrementally change the solution to what it should be and update the objective meanwhile */
    objective_value_new = objective_value_before;
    for( i = 0; i < number_of_touched_parameters; i++ )
    {
      if( parameters[touched_parameters_indices[i]] != parameters_after_touching[i] )
      {
        /* Subtract all edges that are adjacent to the changed node and are in the cut */
        for( j = 0; j < maxcut_number_of_neighbors[touched_parameters_indices[i]]; j++ )
        {
          if( parameters[maxcut_neighbors[touched_parameters_indices[i]][j]] != parameters[touched_parameters_indices[i]] )
            objective_value_new -= maxcut_neighbors_weights[touched_parameters_indices[i]][j];
        }

        /* Change the solution */
        parameters[touched_parameters_indices[i]] = parameters_after_touching[i];

        /* All all edges that are adjacent to the changed node and are in the cut */
        for( j = 0; j < maxcut_number_of_neighbors[touched_parameters_indices[i]]; j++ )
        {
          if( parameters[maxcut_neighbors[touched_parameters_indices[i]][j]] != parameters[touched_parameters_indices[i]] )
            objective_value_new += maxcut_neighbors_weights[touched_parameters_indices[i]][j];
        }
      }
    }

    free( parameters_after_touching );

    *objective_value  = objective_value_new;
    *constraint_value = 0;
  }
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=- Section Initialize -=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/**
 * Performs initializations that are required before starting a run.
 */
void initialize()
{
  number_of_generations = 0;
  number_of_evaluations = 0;

  initializeMemory();

  initializeValueToReach();
  
  initializeRandomNumberGenerator();
  
  initializeProblem( problem_index );

  initializePopulationAndFitnessValues();
}

/**
 * Initializes the memory.
 */
void initializeMemory()
{
  int i;

  offspring_size = population_size;

  population                       = (char **) Malloc( population_size*sizeof( char * ) );
  objective_values                 = (double *) Malloc( population_size*sizeof( double ) );
  constraint_values                = (double *) Malloc( population_size*sizeof( double ) );
  offspring                        = (char **) Malloc( offspring_size*sizeof( char * ) );
  objective_values_offspring       = (double *) Malloc( offspring_size*sizeof( double ) );
  constraint_values_offspring      = (double *) Malloc( offspring_size*sizeof( double ) );
  best_prevgen_solution            = (char *) Malloc( number_of_parameters*sizeof( char ) );
  best_ever_evaluated_solution     = (char *) Malloc( number_of_parameters*sizeof( char ) );
  MI_matrix                        = (double **) Malloc( number_of_parameters*sizeof( double * ) );

  for( i = 0; i < population_size; i++ )
    population[i] = (char *) Malloc( number_of_parameters*sizeof( char ) );

  for( i = 0; i < offspring_size; i++ )
    offspring[i] = (char *) Malloc( number_of_parameters*sizeof( char ) );

  for( i = 0; i < number_of_parameters; i++ )
    MI_matrix[i] = (double *) Malloc( number_of_parameters*sizeof( double ) );

  linkage_model = NULL;
}

/**
 * Initializes the pseudo-random number generator.
 */
void initializeRandomNumberGenerator()
{
  struct timeval tv;
  struct tm *timep;

  while( random_seed_changing == 0 )
  {
    gettimeofday( &tv, NULL );
    timep = localtime (&tv.tv_sec);
    random_seed_changing = timep->tm_hour * 3600 * 1000 + timep->tm_min * 60 * 1000 + timep->tm_sec * 1000 + tv.tv_usec / 1000;
  }

  random_seed = random_seed_changing;
}


void initializeProblem( int index )
{
  switch( index )
  {
    case  5: maxcutProblemInitialization(); break;
    default: break;
  }
}

/**
 * Initializes the population and the objective values by randomly
 * generating n solutions.
 */
void initializePopulationAndFitnessValues()
{
  int     i, j;
  double  obj, con;

  for( i = 0; i < population_size; i++ )
  {
    for( j = 0; j < number_of_parameters; j++ )
      population[i][j] = (randomInt( 2 ) == 1) ? TRUE : FALSE;

    installedProblemEvaluation( problem_index, population[i], &obj, &con );
    objective_values[i]  = obj;
    constraint_values[i] = con;
  }
}

/**
 * Attempts to read the value to reach.
 */
void initializeValueToReach()
{
  char  c, string[100000], filename[1000];
  int   i;
  FILE *file;

    sprintf(filename, "vtr_%s.txt", &file_id);
    printf("%s", filename);
  file = fopen( filename, "r" );
  if( file == NULL )
    use_vtr = 0;
  else
  {
	  use_vtr = 1;
	  
	  i    = 0;
	  c    = fgetc( file );
	  while( (c != '\n') && (c != EOF) && (c != ' ') )
	  {
		  string[i] = (char) c;
		  c         = fgetc( file );
		  i++;
	  }
	  string[i] = '\0';
	  sscanf( string, "%le", &vtr);

	  fclose( file );
  }
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-=- Section Output =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * Writes (appends) statistics about the current generation to a
 * file named "statistics.dat".
 */
void writeGenerationalStatistics()
{
  char    string[10000], filename[1000];
  int     i;
  double  objective_avg, objective_var, objective_best, objective_worst,
          constraint_avg, constraint_var, constraint_best, constraint_worst;
  FILE   *file;

  /* First compute the statistics */
  /* Average, best and worst */
  objective_avg    = 0.0;
  constraint_avg   = 0.0;
  objective_best   = objective_values[0];
  objective_worst  = objective_values[0];
  constraint_best  = constraint_values[0];
  constraint_worst = constraint_values[0];
  for( i = 0; i < population_size; i++ )
  {
    objective_avg += objective_values[i];
    constraint_avg += constraint_values[i];
    if( betterFitness( objective_values[i], constraint_values[i], objective_best, constraint_best ) )
    {
      objective_best  = objective_values[i];
      constraint_best = constraint_values[i];
    }
    if( betterFitness( objective_worst, constraint_worst, objective_values[i], constraint_values[i] ) )
    {
      objective_worst  = objective_values[i];
      constraint_worst = constraint_values[i];
    }
  }
  objective_avg = objective_avg / ((double) population_size);
  constraint_avg = constraint_avg / ((double) population_size);

  /* Variance */
  objective_var  = 0.0;
  constraint_var = 0.0;
  for( i = 0; i < population_size; i++ )
  {
    objective_var += (objective_values[i] - objective_avg)*(objective_values[i] - objective_avg);
    constraint_var += (constraint_values[i] - constraint_avg)*(constraint_values[i] - constraint_avg);
  }
  objective_var = objective_var / ((double) population_size);
  constraint_var = constraint_var / ((double) population_size);

  if( objective_var <= 0.0 )
     objective_var = 0.0;
  if( constraint_var <= 0.0 )
     constraint_var = 0.0;

  /* Then write them */
  file = NULL;

    sprintf(filename, "statistics_%s.dat", &file_id);


    if( number_of_generations == 0 )
  {
    file = fopen( filename, "w" );

    sprintf( string, "# Generation Evaluations      Average-obj.     Variance-obj.         Best-obj.        Worst-obj.        Elite-obj.      Average-con.     Variance-con.         Best-con.        Worst-con.        Elite-con.\n");
    fputs( string, file );
  }
  else
    file = fopen( filename, "a" );

  sprintf( string, "  %10d %11ld %17.10e %17.10e %17.10e %17.10e %17.10e %17.10e %17.10e %17.10e %17.10e %17.10e\n", number_of_generations, number_of_evaluations, objective_avg, objective_var, objective_best, objective_worst, best_prevgen_objective_value, constraint_avg, constraint_var, constraint_best, constraint_worst, best_prevgen_constraint_value );
  fputs( string, file );

  fclose( file );
}

/**
 * Writes the solutions to various files. The filenames
 * contain the generation. If the flag is_final_generation
 * is set the generation number in the filename
 * is replaced with the word "final".
 *
 * population_xxxxx_generation_xxxxx.dat: the population
 * offspring_xxxxx_generation_xxxxx.dat : the offspring
 */
void writeGenerationalSolutions( char is_final_generation )
{
  char  filename[1000], string[10000];
  int   i, j;
  FILE *file;

  /* Population */
  if( is_final_generation )
    sprintf( filename, "population_generation_final_%s.dat", &file_id );
  else
    sprintf( filename, "population_generation_%05d_%s.dat", number_of_generations, &file_id );
  file = fopen( filename, "w" );
  for( i = 0; i < population_size; i++ )
  {
    for( j = 0; j < number_of_parameters; j++ )
    {
      sprintf( string, "%d", (population[i][j] == TRUE) ? 1 : 0 );
      fputs( string, file );
    }
    sprintf( string, "     %17.10e %17.10e\n", objective_values[i], constraint_values[i] );
    fputs( string, file );
  }
  fclose( file );
  
  /* Offspring */
  if( (number_of_generations > 0) && (is_final_generation == FALSE) )
  {
    sprintf( filename, "offspring_generation_%05d_%s.dat", number_of_generations-1, &file_id );
    file = fopen( filename, "w" );

    for( i = 0; i < offspring_size; i++ )
    {
      for( j = 0; j < number_of_parameters; j++ )
      {
        sprintf( string, "%d", (offspring[i][j] == TRUE) ? 1 : 0 );
        fputs( string, file );
      }
      sprintf( string, "     %17.10e %17.10e\n", objective_values_offspring[i], constraint_values_offspring[i] );
      fputs( string, file );
    }
    fclose( file );
  }

  writeGenerationalSolutionsBest( is_final_generation );
}

/**
 * Writes the best solution to a file named
 * best_generation_xxxxx.dat where xxxxx is the
 * generation number. If the flag is_final_generation
 * is set the generation number in the filename
 * is replaced with the word "final".The output
 * file contains the solution,
 * followed by five white spaces and then the
 * single objective value for that solution
 * and its sum of constraint violations.
 */
void writeGenerationalSolutionsBest( char is_final_generation )
{
  char  filename[1000], string[10000];
  int   i, individual_index_best;
  FILE *file;

  /* First find the best of all */
  determineBestSolutionInCurrentPopulation( &individual_index_best );

  /* Then output it */
  if( is_final_generation == TRUE )
    sprintf( filename, "best_generation_final_%s.dat", &file_id);
  else
    sprintf( filename, "best_generation_%05d_%s.dat", number_of_generations, &file_id );
  file = fopen( filename, "w" );

  for( i = 0; i < number_of_parameters; i++ )
  {
    sprintf( string, "%d", (population[individual_index_best][i] == TRUE) ? 1 : 0 );
    fputs( string, file );
  }
  sprintf( string, "     %17.10e %17.10e\n", objective_values[individual_index_best], constraint_values[individual_index_best] );
  fputs( string, file );

  fclose( file );
}

/**
 * Writes the best ever evaluated solution to a file named
 * best_ever.dat. The output file contains the solution,
 * followed by five white spaces and then the
 * single objective value for that solution
 * and its sum of constraint violations.
 */
void writeGenerationalSolutionsBestEver()
{
  char  string[10000], filename[1000];
  int   i;
  FILE *file;

    sprintf(filename, "best_ever_%s.dat", &file_id);

  /* Then output it */
  file = fopen( filename, "w" );

  for( i = 0; i < number_of_parameters; i++ )
  {
    sprintf( string, "%d", (best_ever_evaluated_solution[i] == TRUE) ? 1 : 0 );
    fputs( string, file );
  }
  sprintf( string, "     %17.10e %17.10e\n", best_ever_evaluated_objective_value, best_ever_evaluated_constraint_value );
  fputs( string, file );

  fclose( file );
}

/**
 * Writes the current running time in terms of milliseconds and number of
 * function evaluations so far to a file.
 */
void writeRunningTime( char *filename )
{
  char  string[10000];
  FILE *file;

  file = fopen( filename, "w" );
  sprintf( string, "# Column 1: Total number of milliseconds.\n");
  fputs( string, file );
  sprintf( string, "# Column 2: Total number of evaluations.\n");
  fputs( string, file );
  sprintf( string, "%ld %ld\n", getMilliSecondsRunning(), number_of_evaluations );
  fputs( string, file );
  fclose( file );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=- Section Termination -=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * Returns TRUE if termination should be enforced, FALSE otherwise.
 */
char checkTerminationCondition()
{
  if( maximum_number_of_evaluations >= 0 )
  {
    if( checkNumberOfEvaluationsTerminationCondition() )
      return( TRUE );
  }
  
  if( use_vtr )
  {
    if( checkVTRTerminationCondition() )
      return( TRUE );
  }

  if( checkFitnessVarianceTermination() )
    return( TRUE );

  return( FALSE );
}

/**
 * Returns TRUE if the maximum number of evaluations
 * has been reached, FALSE otherwise.
 */
char checkNumberOfEvaluationsTerminationCondition()
{
  if( number_of_evaluations >= maximum_number_of_evaluations )
    return( TRUE );

  return( FALSE );
}

/**
 * Returns TRUE if the value-to-reach has been reached.
 */
char checkVTRTerminationCondition()
{
  int individual_index_best;

  determineBestSolutionInCurrentPopulation( &individual_index_best );

  if( constraint_values[individual_index_best] == 0 && objective_values[individual_index_best] >= vtr  )
    return( TRUE );
    
  return( FALSE );
}

/**
 * Determines which solution is the best of all solutions in the current population.
 */
void determineBestSolutionInCurrentPopulation( int *index_of_best )
{
  int i;

  *index_of_best = 0;
  for( i = 0; i < population_size; i++ )
  {
    if( betterFitness( objective_values[i], constraint_values[i],
                       objective_values[*index_of_best], constraint_values[*index_of_best] ) )
      *index_of_best = i;
  }
}

/**
 * Checks whether the fitness variance
 * has become too small (user-defined tolerance).
 */
char checkFitnessVarianceTermination()
{
  int    i;
  double objective_avg, objective_var;
  
  objective_avg = 0.0;
  for( i = 0; i < population_size; i++ )
    objective_avg  += objective_values[i];
  objective_avg  = objective_avg / ((double) population_size);

  objective_var = 0.0;
  for( i = 0; i < population_size; i++ )
    objective_var  += (objective_values[i]-objective_avg)*(objective_values[i]-objective_avg);
  objective_var  = objective_var / ((double) population_size);

  if( objective_var <= 0.0 )
     objective_var = 0.0;

  if( objective_var <= fitness_variance_tolerance )
    return( TRUE );

  return( FALSE );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*=-=-=-=-=-=-=-=-=-=-= Section Survivor Selection =-=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * Determines the solutions that finally survive the generation.
 */
void selectFinalSurvivors()
{
  int i, j;

  for( i = 0; i < population_size; i++ )
  {
    for( j = 0; j < number_of_parameters; j++ )
      population[i][j] = offspring[i][j];
    objective_values[i]  = objective_values_offspring[i];
    constraint_values[i] = constraint_values_offspring[i];
  }
}

/**
 * Determines the best solution in the population
 * and compares it to the best solution stored as
 * the best solution from all previous generations.
 * If the current best solution is better that the
 * stored solution, it replaces that solution.
 */
void updateBestPrevGenSolution()
{
  char replace_best_prevgen;
  int  i, index, individual_index_best;

  /* First find the best of all */
  determineBestSolutionInCurrentPopulation( &index );
  individual_index_best = index;

  replace_best_prevgen = FALSE;
  if( number_of_generations == 0 )
    replace_best_prevgen = TRUE;
  else
  {
    if( betterFitness( objective_values[individual_index_best], constraint_values[individual_index_best],
                       best_prevgen_objective_value, best_prevgen_constraint_value ) )
      replace_best_prevgen = TRUE;
  }

  if( replace_best_prevgen == TRUE )
  {
    for( i = 0; i < number_of_parameters; i++ )
      best_prevgen_solution[i] = population[individual_index_best][i];
    best_prevgen_objective_value  = objective_values[individual_index_best];
    best_prevgen_constraint_value = constraint_values[individual_index_best];
    no_improvement_stretch       = 0;
  }
  else
    no_improvement_stretch++;
}

/**
 * Returns 1 if x is better than y, 0 otherwise.
 * x is not better than y unless:
 * - x and y are both infeasible and x has a smaller sum of constraint violations, or
 * - x is feasible and y is not, or
 * - x and y are both feasible and x has a larger objective value than y
 */
char betterFitness( double objective_value_x, double constraint_value_x, double objective_value_y, double constraint_value_y )
{
  char result;
  
  result = FALSE;

  if( constraint_value_x > 0 ) /* x is infeasible */
  {
    if( constraint_value_y > 0 ) /* Both are infeasible */
    {
      if( constraint_value_x < constraint_value_y )
       result = TRUE;
    }
  }
  else /* x is feasible */
  {
    if( constraint_value_y > 0 ) /* x is feasible and y is not */
      result = TRUE;
    else /* Both are feasible */
    {
      if( objective_value_x > objective_value_y )
        result = TRUE;
    }
  }

  return( result );
}

/**
 * Returns 1 if x is equally preferable to y, 0 otherwise.
 */
char equalFitness( double objective_value_x, double constraint_value_x, double objective_value_y, double constraint_value_y )
{
  char result;
  
  result = FALSE;

  if( (constraint_value_x == constraint_value_y) && (objective_value_x == objective_value_y) )
    result = TRUE;

  return( result );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-= Section Variation -==-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * Learns the linkage tree from the population. Then fills up the offspring pool
 * by applying genepool optimal mixing (GOM) to every population member.
 */
void makeOffspring()
{
  if( use_univariate_fos )
    initializeUnivariateFOS();
  else
    learnLinkageTree();

  if( print_linkage_model_contents )
    printLinkageModel();

  generateAndEvaluateNewSolutionsToFillOffspring();
}

void initializeUnivariateFOS()
{
  int i, j, *order;
    if( linkage_model != NULL )
  {
      for( i = 0; i < linkage_model_length; i++ ) {
          free( linkage_model[i] );
      }
      free( linkage_model );
      free( linkage_model_number_of_indices );
  }

    linkage_model_length = number_of_parameters; // TODO: length of a univariate factorization (Done)

    linkage_model_number_of_indices = (int*) Malloc( linkage_model_length * sizeof(int) );

    order = randomPermutation(linkage_model_length);

    linkage_model = (int **) Malloc( (linkage_model_length)*sizeof( int *) );

  // TODO: Initialize linkage model to the univariate factorization
  for( i = 0; i < linkage_model_length; i++ )
  {
      linkage_model_number_of_indices[i] = 1; // TODO: Size of the ith FOS element (Done, 1 because only 1 element)
      linkage_model[i] = (int *) Malloc(linkage_model_number_of_indices[i] * sizeof(int ));

    for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
	{
      linkage_model[i][j] = order[i]; // TODO: Fill the jth position of the ith FOS element (Done, with random order)
    }
  }
}

/**
 * Learns the linkage tree using the reciprocal nearest neighbor approach.
 */
void learnLinkageTree()
{
  char   done;
  int    i, j, k, a, r0, r1, *indices, *order,
         linkage_model_index, factor_size, **mpm_new, *mpm_new_number_of_indices, mpm_new_length,
        *NN_chain, NN_chain_length;
  double p, *cumulative_probabilities, **S_matrix, mul0, mul1;

  /* Compute joint entropy matrix */
  for( i = 0; i < number_of_parameters; i++ )
  {
    for( j = i+1; j < number_of_parameters; j++ )
    {
      indices                  = (int *) Malloc( 2*sizeof( int ) );
      indices[0]               = i;
      indices[1]               = j;
      cumulative_probabilities = estimateParametersForSingleBinaryMarginal( indices, 2, &factor_size );

      MI_matrix[i][j] = 0.0;
      for( k = 0; k < factor_size; k++ )
      {
        if( k == 0 )
          p = cumulative_probabilities[k];
        else
          p = cumulative_probabilities[k]-cumulative_probabilities[k-1];
        if( p > 0 )
          MI_matrix[i][j] += -p*log2(p);
      }

      MI_matrix[j][i] = MI_matrix[i][j];

      free( indices );
      free( cumulative_probabilities );
    }
    indices                  = (int *) Malloc( 1*sizeof( int ) );
    indices[0]               = i;
    cumulative_probabilities = estimateParametersForSingleBinaryMarginal( indices, 1, &factor_size );

    MI_matrix[i][i] = 0.0;
    for( k = 0; k < factor_size; k++ )
    {
      if( k == 0 )
        p = cumulative_probabilities[k];
      else
        p = cumulative_probabilities[k]-cumulative_probabilities[k-1];
       if( p > 0 )
       MI_matrix[i][i] += -p*log2(p);
    }

    free( indices );
    free( cumulative_probabilities );
  }

  /* Then transform into mutual information matrix MI(X,Y)=H(X)+H(Y)-H(X,Y) */
  for( i = 0; i < number_of_parameters; i++ )
    for( j = i+1; j < number_of_parameters; j++ )
    {
      MI_matrix[i][j] = MI_matrix[i][i] + MI_matrix[j][j] - MI_matrix[i][j];
      MI_matrix[j][i] = MI_matrix[i][j];
    }

  /* Initialize MPM to the univariate factorization */
  order                 = randomPermutation( number_of_parameters );
  mpm                   = (int **) Malloc( number_of_parameters*sizeof( int * ) );
  mpm_number_of_indices = (int *) Malloc( number_of_parameters*sizeof( int ) );
  mpm_length            = number_of_parameters;
  for( i = 0; i < number_of_parameters; i++ )
  {
    indices                  = (int *) Malloc( 1*sizeof( int ) );
    indices[0]               = order[i];
    mpm[i]                   = indices;
    mpm_number_of_indices[i] = 1;
  }
  free( order );

  /* Initialize linkage_model to the initial MPM */
  if( linkage_model != NULL )
  {
    for( i = 0; i < linkage_model_length; i++ )
      free( linkage_model[i] );
    free( linkage_model );
    free( linkage_model_number_of_indices );
  }
  linkage_model                   = (int **) Malloc( (number_of_parameters+number_of_parameters-1)*sizeof( int * ) );
  linkage_model_number_of_indices = (int *) Malloc( (number_of_parameters+number_of_parameters-1)*sizeof( int ) );
  linkage_model_length            = number_of_parameters+number_of_parameters-1;
  linkage_model_index             = 0;
  for( i = 0; i < mpm_length; i++ )
  {
    linkage_model[linkage_model_index]                   = mpm[i];
    linkage_model_number_of_indices[linkage_model_index] = mpm_number_of_indices[i];
    linkage_model_index++;
  }

  /* Initialize similarity matrix */
  S_matrix = (double **) Malloc( number_of_parameters*sizeof( double * ) );
  for( i = 0; i < number_of_parameters; i++ )
    S_matrix[i] = (double *) Malloc( number_of_parameters*sizeof( double ) );
  for( i = 0; i < mpm_length; i++ )
    for( j = 0; j < mpm_length; j++ )
      S_matrix[i][j] = MI_matrix[mpm[i][0]][mpm[j][0]];
  for( i = 0; i < mpm_length; i++ )
    S_matrix[i][i] = 0;

  NN_chain        = (int *) Malloc( (number_of_parameters+2)*sizeof( int ) );
  NN_chain_length = 0;
  done            = FALSE;
  while( done == FALSE )
  {
    if( NN_chain_length == 0 )
    {
      NN_chain[NN_chain_length] = randomInt( mpm_length );
      NN_chain_length++;
    }

    while( NN_chain_length < 3 )
    {
      NN_chain[NN_chain_length] = determineNearestNeighbour( NN_chain[NN_chain_length-1], S_matrix, mpm_length );
      NN_chain_length++;
    }

    while( NN_chain[NN_chain_length-3] != NN_chain[NN_chain_length-1] )
    {
      NN_chain[NN_chain_length] = determineNearestNeighbour( NN_chain[NN_chain_length-1], S_matrix, mpm_length );
      if( ((S_matrix[NN_chain[NN_chain_length-1]][NN_chain[NN_chain_length]] == S_matrix[NN_chain[NN_chain_length-1]][NN_chain[NN_chain_length-2]])) && (NN_chain[NN_chain_length] != NN_chain[NN_chain_length-2]) )
        NN_chain[NN_chain_length] = NN_chain[NN_chain_length-2];
      NN_chain_length++;
      if( NN_chain_length > number_of_parameters )
        break;
    }
    r0 = NN_chain[NN_chain_length-2];
    r1 = NN_chain[NN_chain_length-1];
    if( r0 > r1 )
    {
      a  = r0;
      r0 = r1;
      r1 = a;
    }
    NN_chain_length -= 3;

    if( r1 < mpm_length ) /* This test is required for exceptional cases in which the nearest-neighbor ordering has changed within the chain while merging within that chain. */
    {
      indices = (int *) Malloc( (mpm_number_of_indices[r0]+mpm_number_of_indices[r1])*sizeof( int ) );
  
      i = 0;
      for( j = 0; j < mpm_number_of_indices[r0]; j++ )
      {
        indices[i] = mpm[r0][j];
        i++;
      }
      for( j = 0; j < mpm_number_of_indices[r1]; j++ )
      {
        indices[i] = mpm[r1][j];
        i++;
      }
    
      linkage_model[linkage_model_index]                   = indices;
      linkage_model_number_of_indices[linkage_model_index] = mpm_number_of_indices[r0]+mpm_number_of_indices[r1];
      linkage_model_index++;
  
      mul0 = ((double) mpm_number_of_indices[r0])/((double) mpm_number_of_indices[r0]+mpm_number_of_indices[r1]);
      mul1 = ((double) mpm_number_of_indices[r1])/((double) mpm_number_of_indices[r0]+mpm_number_of_indices[r1]);
      for( i = 0; i < mpm_length; i++ )
      {
        if( (i != r0) && (i != r1) )
        {
          S_matrix[i][r0] = mul0*S_matrix[i][r0] + mul1*S_matrix[i][r1];
          S_matrix[r0][i] = S_matrix[i][r0];
        }
      }
  
      mpm_new                   = (int **) Malloc( (mpm_length-1)*sizeof( int * ) );
      mpm_new_number_of_indices = (int *) Malloc( (mpm_length-1)*sizeof( int ) );
      mpm_new_length            = mpm_length-1;
      for( i = 0; i < mpm_new_length; i++ )
      {
        mpm_new[i]                   = mpm[i];
        mpm_new_number_of_indices[i] = mpm_number_of_indices[i];
      }
  
      mpm_new[r0]                   = indices;
      mpm_new_number_of_indices[r0] = mpm_number_of_indices[r0]+mpm_number_of_indices[r1];
      if( r1 < mpm_length-1 )
      {
        mpm_new[r1]                   = mpm[mpm_length-1];
        mpm_new_number_of_indices[r1] = mpm_number_of_indices[mpm_length-1];
  
        for( i = 0; i < r1; i++ )
        {
          S_matrix[i][r1] = S_matrix[i][mpm_length-1];
          S_matrix[r1][i] = S_matrix[i][r1];
        }
  
        for( j = r1+1; j < mpm_new_length; j++ )
        {
          S_matrix[r1][j] = S_matrix[j][mpm_length-1];
          S_matrix[j][r1] = S_matrix[r1][j];
        }
      }
  
      for( i = 0; i < NN_chain_length; i++ )
      {
        if( NN_chain[i] == mpm_length-1 )
        {
          NN_chain[i] = r1;
          break;
        }
      }
  
      free( mpm );
      free( mpm_number_of_indices );
      mpm                   = mpm_new;
      mpm_number_of_indices = mpm_new_number_of_indices;
      mpm_length            = mpm_new_length;
  
      if( mpm_length == 1 )
        done = TRUE;
    }
  }

  free( NN_chain );

  free( mpm_new );
  free( mpm_number_of_indices );

  for( i = 0; i < number_of_parameters; i++ )
    free( S_matrix[i] );
  free( S_matrix );
}

/**
 * Estimates the cumulative probability distribution of a
 * single binary marginal (EDA legacy code).
 */
double *estimateParametersForSingleBinaryMarginal( int *indices, int number_of_indices, int *factor_size )
{
  int     i, j, index, power_of_two;
  double *result;

  *factor_size = (int) pow( 2, number_of_indices );
  result       = (double *) Malloc( (*factor_size)*sizeof( double ) );

  for( i = 0; i < (*factor_size); i++ )
    result[i] = 0.0;

  for( i = 0; i < population_size; i++ )
  {
    index        = 0;
    power_of_two = 1;
    for( j = number_of_indices-1; j >= 0; j-- )
    {
      index += (population[i][indices[j]] == TRUE) ? power_of_two : 0;
      power_of_two *= 2;
    }

    result[index] += 1.0;
  }

  for( i = 0; i < (*factor_size); i++ )
    result[i] /= (double) population_size;

  for( i = 1; i < (*factor_size); i++ )
    result[i] += result[i-1];

  result[(*factor_size)-1] = 1.0;

  return( result );
}

/**
 * Determines nearest neighbour according to similarity values.
 */
int determineNearestNeighbour( int index, double **S_matrix, int mpm_length )
{
  int i, result;

  result = 0;
  if( result == index )
    result++;
  for( i = 1; i < mpm_length; i++ )
  {
    if( ((S_matrix[index][i] > S_matrix[index][result]) || ((S_matrix[index][i] == S_matrix[index][result]) && (mpm_number_of_indices[i] < mpm_number_of_indices[result]))) && (i != index) )
      result = i;
  }

  return( result );
}

/**
 * Prints the linkage model to standard output.
 */
void printLinkageModel()
{
  int i, j;

  for( i = 0; i < linkage_model_length; i++ )
  {
    printf("[");
    for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
    {
      printf("%d",linkage_model[i][j]);
      if( j < linkage_model_number_of_indices[i]-1 )
        printf(" ");
    }
    printf("]\n");
  }
  printf("\n");
  fflush( stdout );
}

/**
 * Computes the two-log of x.
 */
double math_log_two = log(2.0);
double log2( double x )
{
  return( log(x) / math_log_two );
}

/**
 * Generates new solutions by applying GOM to every population
 * member. The new solutions are placed in a separate offspring pool.
 */
void generateAndEvaluateNewSolutionsToFillOffspring()
{
  char  *solution;
  int    i, j;
  double obj, con;

  for( i = 0; i < offspring_size; i++ )
  {
    solution = generateNewSolution( i%population_size, &obj, &con );

    for( j = 0; j < number_of_parameters; j++ )
      offspring[i][j] = solution[j];

    objective_values_offspring[i]  = obj;
    constraint_values_offspring[i] = con;

    free( solution );
  }
}

/**
 * Generates and returns a single new solution by applying
 * genepool optimal mixing (GOM) to a specified population
 * member.
 */
char *generateNewSolution( int which, double *obj, double *con )
{
  char   *result, *backup, is_unchanged;
  short   solution_has_changed;
  int     i, j, donor_index;
  double  obj_backup, con_backup;

  solution_has_changed = 0;

  result = (char *) Malloc( number_of_parameters*sizeof( char ) );
  for( i = 0; i < number_of_parameters; i++ )
    result[i] = population[which][i];

  *obj = objective_values[which];
  *con = constraint_values[which];

  backup = (char *) Malloc( number_of_parameters*sizeof( char ) );
  for( i = 0; i < number_of_parameters; i++ )
    backup[i] = result[i];

  obj_backup = *obj;
  con_backup = *con;

  /* Phase 1: optimal mixing with random donors */
  for( i = linkage_model_length-2; i >= 0; i-- )
  {
    /* Pick random donor. */
    donor_index = randomInt( population_size );

    /* Copy donor material for indices in i-th linkage subset. */
    for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
      result[linkage_model[i][j]] = population[donor_index][linkage_model[i][j]];

    /* Test if the change is for the better (don't test at all if no change occurred). */
    is_unchanged = TRUE;
    for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
    {
      if( backup[linkage_model[i][j]] != result[linkage_model[i][j]] )
      {
        is_unchanged = FALSE;
        break;
      }
    }
    if( is_unchanged == FALSE )
    {
      installedProblemEvaluation( problem_index, result, obj, con );

      if( betterFitness( *obj, *con, obj_backup, con_backup ) || equalFitness( *obj, *con, obj_backup, con_backup ) )
      {
        for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
          backup[linkage_model[i][j]] = result[linkage_model[i][j]];

        obj_backup = *obj;
        con_backup = *con;

        solution_has_changed = 1;
      }
      else
      {
        for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
          result[linkage_model[i][j]] = backup[linkage_model[i][j]];

        *obj = obj_backup;
        *con = con_backup;
      }
    }
  }

  /* Phase 2 (Forced Improvement): optimal mixing with best solution from previous generations */
  if( (solution_has_changed != 1) || (no_improvement_stretch > (1+(log(population_size)/log(10)))) )
  {
    solution_has_changed = 0;
    for( i = linkage_model_length-2; i >= 0; i-- )
    {
      /* Convert elite solution to binary representation and set factor variables. */
      for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
        result[linkage_model[i][j]] = best_prevgen_solution[linkage_model[i][j]];

      /* Test if the change is for the better */
      is_unchanged = TRUE;
      for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
      {
        if( backup[linkage_model[i][j]] != result[linkage_model[i][j]] )
        {
          is_unchanged = FALSE;
          break;
        }
      }
      if( is_unchanged == FALSE )
      {
        installedProblemEvaluation( problem_index, result, obj, con );

        if( betterFitness( *obj, *con, obj_backup, con_backup ) )
        {
          for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
            backup[linkage_model[i][j]] = result[linkage_model[i][j]];

          obj_backup = *obj;
          con_backup = *con;

          solution_has_changed = 1;
        }
        else
        {
          for( j = 0; j < linkage_model_number_of_indices[i]; j++ )
            result[linkage_model[i][j]] = backup[linkage_model[i][j]];

          *obj = obj_backup;
          *con = con_backup;
        }
      }
      if( solution_has_changed == 1 )
        break;
    }

    if( solution_has_changed != 1 )
    {
      if( betterFitness( best_prevgen_objective_value, best_prevgen_constraint_value, *obj, *con ) )
        solution_has_changed = 1;

      for( i = 0; i < number_of_parameters; i++ )
        result[i] = best_prevgen_solution[i];
      *obj = best_prevgen_objective_value;
      *con = best_prevgen_constraint_value;
    }
  }

  free( backup );

  return( result );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=- Section Ezilaitini -=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/**
 * Undoes initialization procedure by freeing up memory.
 */
void ezilaitiniMemory( void )
{
  int i;

  for( i = 0; i < number_of_parameters; i++ )
    free( MI_matrix[i] );

  for( i = 0; i < offspring_size; i++ )
    free( offspring[i] );

  for( i = 0; i < population_size; i++ )
    free( population[i] );

  free( population );
  free( objective_values );
  free( constraint_values );
  free( offspring );
  free( objective_values_offspring );
  free( constraint_values_offspring );
  free( best_prevgen_solution );
  free( best_ever_evaluated_solution );
  free( MI_matrix );

  if( linkage_model != NULL )
  {
    for( i = 0; i < linkage_model_length; i++ )
      free( linkage_model[i] );
    free( linkage_model );
    free( linkage_model_number_of_indices );
  }
}

void ezilaitiniProblem( int index )
{
  switch( index )
  {
    case  6: maxcutProblemNoitazilaitini(); break;
    default: break;
  }
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=- Section Run -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/**
 * Runs the GOMEA.
 */
void run()
{
  struct timeval tv;
  struct tm *timep;

  gettimeofday( &tv, NULL );
  timep = localtime (&tv.tv_sec);
  timestamp_start = timep->tm_hour * 3600 * 1000 + timep->tm_min * 60 * 1000 + timep->tm_sec * 1000 + tv.tv_usec / 1000;
  vtr_hit_has_happened = 0;

  initialize();

  if( print_verbose_overview )
    printVerboseOverview();

  updateBestPrevGenSolution();

  while( !checkTerminationCondition() )
  {
    if( write_generational_statistics )
      writeGenerationalStatistics();

    if( write_generational_solutions )
      writeGenerationalSolutions( FALSE );

    makeOffspring();

    selectFinalSurvivors();

    number_of_generations++;

    updateBestPrevGenSolution();
  }
  char filename[1000];

    sprintf(filename, "total_running_time_%s.dat", &file_id);


    writeRunningTime( filename);

  writeGenerationalStatistics();

  if( write_generational_solutions )
    writeGenerationalSolutions( TRUE );

  writeGenerationalSolutionsBestEver();

  ezilaitiniProblem( problem_index );
  
  ezilaitiniMemory();
}

/**
 * Returns the number of milliseconds since the initial timestamp
 * (after interpreting the commandline).
 */
long getMilliSecondsRunning()
{
  struct timeval tv;
  struct tm *timep;
  long   timestamp_now, difference;

  gettimeofday( &tv, NULL );
  timep = localtime (&tv.tv_sec);
  timestamp_now = timep->tm_hour * 3600 * 1000 + timep->tm_min * 60 * 1000 + timep->tm_sec * 1000 + tv.tv_usec / 1000;

  difference = timestamp_now-timestamp_start;
  if( difference < 0 )
    difference += 86399999;

  return( difference );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/





/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=- Section Main -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/**
 * The main function:
 * - interpret parameters on the command line
 * - run GOMEA with the interpreted parameters
 */
int main( int argc, char **argv )
{
  interpretCommandLine( argc, argv );

  run();

  return( 0 );
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
